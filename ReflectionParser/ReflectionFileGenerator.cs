using System.Text;
using ReflectionParser.Public;

namespace ReflectionParser;

/// <summary>
/// 反射代码生成器。
/// 基于 Token 流解析头文件，提取类/属性/函数/枚举信息。
/// 策略：不做完整语法分析，而是基于有限状态机在 Token 流上做模式匹配。
/// </summary>
public class ReflectionFileGenerator
{
    private readonly string _headerText;
    private readonly string _filePath;
    private readonly string _fileName;

    // 解析结果
    private string _namespaceName = "";
    private AbstractStructures.ClassStructure? _currentClass;
    private AbstractStructures.EnumStructure? _currentEnum;
    private PackageType _currentAccessModifier = PackageType.Private; // C++ class 默认为 private

    private readonly List<AbstractStructures.ClassStructure> _classes = new();
    private readonly List<AbstractStructures.EnumStructure> _enums = new();

    public ReflectionFileGenerator(int index)
    {
        _filePath = PublicVariables.ReflectionHeaders[index].HeaderPath;
        _headerText = PublicVariables.ReflectionHeaders[index].HeaderFileText;
        _fileName = Path.GetFileNameWithoutExtension(_filePath);
    }

    /// <summary>
    /// 执行解析
    /// </summary>
    public void Parse()
    {
        var tokenizer = new Tokenizer(_headerText);
        var tokens = tokenizer.Tokenize();

        _namespaceName = "";
        _currentClass = null;
        _currentEnum = null;
        _currentAccessModifier = PackageType.Private;
        _classes.Clear();
        _enums.Clear();

        ParseTopLevel(tokens, 0);
    }

    // ──────────────────────────────────────────────
    // 顶层解析：namespace / SCLASS / SENUM
    // ──────────────────────────────────────────────
    private int ParseTopLevel(List<Token> tokens, int startIndex)
    {
        int i = startIndex;
        while (i < tokens.Count)
        {
            var t = tokens[i];

            // 跳过预处理器指令（#include, #pragma 等）
            // 注意：# 被 Token 化为单独符号，后面跟着 include/pragma/define/if/endif 等
            if (t.Type == TokenType.Word && t.Value == "#")
            {
                i++;
                // 看下一个 Token 是否是指令名
                if (i < tokens.Count && tokens[i].Type == TokenType.Word)
                {
                    string directive = tokens[i].Value;
                    if (directive == "include" || directive == "pragma" || directive == "define" || directive == "error" || directive == "warning" || directive == "line")
                    {
                        i = SkipLine(tokens, i);
                        continue;
                    }
                    if (directive == "if" || directive == "ifdef" || directive == "ifndef")
                    {
                        i = SkipPreprocessorBlock(tokens, i);
                        continue;
                    }
                    if (directive == "endif" || directive == "else" || directive == "elif")
                    {
                        // 这些不应该在顶层出现，但安全跳过
                        i++;
                        continue;
                    }
                }
                continue;
            }

            // namespace
            if (t.Type == TokenType.Word && t.Value == "namespace")
            {
                i = ParseNamespace(tokens, i);
                continue;
            }

            // SCLASS() — 类反射标记
            if (t.Type == TokenType.Word && t.Value == "SCLASS")
            {
                // 跳过 SCLASS() 宏
                i = SkipMacroCall(tokens, i);
                // 下一个应该是 class/struct
                i = ParseClass(tokens, i);
                continue;
            }

            // SENUM() — 枚举反射标记
            if (t.Type == TokenType.Word && t.Value == "SENUM")
            {
                i = SkipMacroCall(tokens, i);
                i = ParseEnum(tokens, i);
                continue;
            }

            i++;
        }
        return i;
    }

    // ──────────────────────────────────────────────
    // namespace 解析
    // ──────────────────────────────────────────────
    private int ParseNamespace(List<Token> tokens, int startIndex)
    {
        // namespace Name {
        // 或 namespace Name = ...;（别名，跳过）
        int i = startIndex + 1; // 跳过 "namespace"

        if (i >= tokens.Count) return i;

        // 获取 namespace 名
        string nsName = "";
        if (tokens[i].Type == TokenType.Word)
        {
            nsName = tokens[i].Value;
            i++;
        }

        // 检查是否有 = （命名空间别名，跳过）
        if (i < tokens.Count && tokens[i].Type == TokenType.Assign)
        {
            return SkipToSemicolon(tokens, i);
        }

        // 期望 {
        if (i < tokens.Count && tokens[i].Type == TokenType.OpenBrace)
        {
            // 保存旧的 namespace 并压栈
            string oldNs = _namespaceName;
            if (!string.IsNullOrEmpty(_namespaceName))
                _namespaceName = _namespaceName + "::" + nsName;
            else
                _namespaceName = nsName;

            i++; // 跳过 {

            // 递归解析 namespace 内部
            int braceDepth = 1;
            while (i < tokens.Count && braceDepth > 0)
            {
                var t = tokens[i];

                if (t.Type == TokenType.CloseBrace) { braceDepth--; if (braceDepth == 0) { i++; break; } i++; continue; }
                if (t.Type == TokenType.OpenBrace) { braceDepth++; i++; continue; }

                // 处理内部的 SCLASS / SENUM / namespace（嵌套 namespace）
                if (t.Type == TokenType.Word)
                {
                    if (t.Value == "SCLASS")
                    {
                        i = SkipMacroCall(tokens, i);
                        i = ParseClass(tokens, i);
                        continue;
                    }
                    if (t.Value == "SENUM")
                    {
                        i = SkipMacroCall(tokens, i);
                        i = ParseEnum(tokens, i);
                        continue;
                    }
                    if (t.Value == "namespace")
                    {
                        i = ParseNamespace(tokens, i);
                        continue;
                    }
                    // 预处理器（# 被 Token 化为单独符号）
                    if (t.Value == "#")
                    {
                        i++;
                        // 跳过指令名和参数
                        while (i < tokens.Count && tokens[i].Type != TokenType.Semicolon &&
                               tokens[i].Type != TokenType.OpenBrace && tokens[i].Type != TokenType.CloseBrace)
                            i++;
                        continue;
                    }
                }

                i++;
            }

            _namespaceName = oldNs; // 恢复
        }

        return i;
    }

    // ──────────────────────────────────────────────
    // 类解析
    // ──────────────────────────────────────────────
    private int ParseClass(List<Token> tokens, int startIndex)
    {
        int i = startIndex;
        if (i >= tokens.Count) return i;

        // 期望: class/struct ClassName [final] [:
        //   [public/private/protected] BaseClass, ...] {
        if (tokens[i].Type != TokenType.Word ||
            (tokens[i].Value != "class" && tokens[i].Value != "struct"))
            return i;

        bool isStruct = tokens[i].Value == "struct";
        i++; // 跳过 class/struct

        // 获取类名
        if (i >= tokens.Count || tokens[i].Type != TokenType.Word)
            return i;

        string className = tokens[i].Value;
        i++;

        var classInfo = new AbstractStructures.ClassStructure
        {
            NamespaceName = _namespaceName,
            ClassName = className,
            InheritanceInfo = new AbstractStructures.InheritanceInfo { InheritanceNameList = new List<string>() }
        };
        _currentClass = classInfo;
        // class 默认为 private，struct 默认为 public
        _currentAccessModifier = isStruct ? PackageType.Public : PackageType.Private;

        // 跳过可能的 final / 属性等关键字
        while (i < tokens.Count && tokens[i].Type == TokenType.Word &&
               (tokens[i].Value == "final" || tokens[i].Value == "abstract" ||
                tokens[i].Value == "sealed" || tokens[i].Value == "override"))
            i++;

        // 检查继承: 冒号
        if (i < tokens.Count && tokens[i].Type == TokenType.Colon)
        {
            i++; // 跳过 :
            // 解析基类列表直到 {
            while (i < tokens.Count && tokens[i].Type != TokenType.OpenBrace)
            {
                // 跳过访问修饰符
                if (tokens[i].Type == TokenType.Word &&
                    (tokens[i].Value == "public" || tokens[i].Value == "private" ||
                     tokens[i].Value == "protected"))
                {
                    i++; // 跳过访问修饰符
                    if (i < tokens.Count && tokens[i].Type == TokenType.Colon)
                        i++; // 跳过冒号
                    continue;
                }
                if (tokens[i].Type == TokenType.Word)
                {
                    // 可能是基类名或 virtual
                    if (tokens[i].Value != "virtual")
                    {
                        classInfo.InheritanceInfo.InheritanceNameList.Add(tokens[i].Value);
                    }
                    i++;
                    continue;
                }
                if (tokens[i].Type == TokenType.Comma) { i++; continue; }
                if (tokens[i].Type == TokenType.DoubleColon) { i++; continue; }
                if (tokens[i].Type == TokenType.OpenBracket) { i = SkipBrackets(tokens, i); continue; }
                if (tokens[i].Type == TokenType.CloseBracket) { i++; continue; }
                i++;
            }
        }

        // 期望 {
        if (i < tokens.Count && tokens[i].Type == TokenType.OpenBrace)
        {
            i++; // 跳过 {

            // 解析类体
            int braceDepth = 1;
            while (i < tokens.Count && braceDepth > 0)
            {
                var t = tokens[i];

                if (t.Type == TokenType.CloseBrace) { braceDepth--; if (braceDepth == 0) { i++; break; } i++; continue; }
                if (t.Type == TokenType.OpenBrace) { braceDepth++; i++; continue; }

                // 访问修饰符: public: / private: / protected:
                if (t.Type == TokenType.Word &&
                    (t.Value == "public" || t.Value == "private" || t.Value == "protected"))
                {
                    int peek = i + 1;
                    if (peek < tokens.Count && tokens[peek].Type == TokenType.Colon)
                    {
                        _currentAccessModifier = t.Value switch
                        {
                            "public" => PackageType.Public,
                            "private" => PackageType.Private,
                            "protected" => PackageType.Protected,
                            _ => _currentAccessModifier
                        };
                        i += 2;
                        continue;
                    }
                }

                // GENERATED_BODY()
                if (t.Type == TokenType.Word && t.Value == "GENERATED_BODY")
                {
                    i = SkipMacroCall(tokens, i);
                    continue;
                }

                // SFUNCTION()
                if (t.Type == TokenType.Word && t.Value == "SFUNCTION")
                {
                    i = SkipMacroCall(tokens, i);
                    i = ParseFunction(tokens, i);
                    continue;
                }

                // SPROPERTY()
                if (t.Type == TokenType.Word && t.Value == "SPROPERTY")
                {
                    i = SkipMacroCall(tokens, i);
                    i = ParseProperty(tokens, i);
                    continue;
                }

                // 预处理器（# 被 Token 化为单独符号）
                if (t.Type == TokenType.Word && t.Value == "#")
                {
                    i++;
                    while (i < tokens.Count && tokens[i].Type != TokenType.Semicolon &&
                           tokens[i].Type != TokenType.OpenBrace && tokens[i].Type != TokenType.CloseBrace)
                        i++;
                    continue;
                }

                // 友元声明 friend class X;
                if (t.Type == TokenType.Word && t.Value == "friend")
                {
                    i = SkipToSemicolon(tokens, i);
                    continue;
                }

                // 构造函数/析构函数/普通成员函数（没有 SFUNCTION 标记的）— 跳过
                // 简单启发式：如果是 类名(...) 或 ~类名(...)，跳过整个声明
                if (t.Type == TokenType.Word && t.Value == className)
                {
                    i = SkipFunctionDeclaration(tokens, i);
                    continue;
                }
                if (t.Type == TokenType.Tilde)
                {
                    i = SkipFunctionDeclaration(tokens, i);
                    continue;
                }

                // using 声明: using X = ...; 或 using namespace X;
                if (t.Type == TokenType.Word && t.Value == "using")
                {
                    i = SkipToSemicolon(tokens, i);
                    continue;
                }

                // static_assert
                if (t.Type == TokenType.Word && t.Value == "static_assert")
                {
                    i = SkipToSemicolon(tokens, i);
                    continue;
                }

                // 模板友元
                if (t.Type == TokenType.Word && t.Value == "template")
                {
                    i = SkipTemplateDeclaration(tokens, i);
                    continue;
                }

                i++;
            }

            if (classInfo.InheritanceInfo.InheritanceNameList.Count == 0)
                classInfo.InheritanceInfo.InheritanceNameList = new List<string>();

            _classes.Add(classInfo);
        }

        _currentClass = null;
        return i;
    }

    // ──────────────────────────────────────────────
    // 属性解析
    // ──────────────────────────────────────────────
    private int ParseProperty(List<Token> tokens, int startIndex)
    {
        // 策略：从当前位置开始，读取到分号为止。
        // 然后从后往前推：最后一个 Word 是变量名，前面的都是类型。
        // 处理复杂类型时要做括号匹配。
        int i = startIndex;
        if (i >= tokens.Count) return i;

        // 收集所有 Token 直到分号或 {
        int start = i;
        int semicolonPos = -1;
        int braceDepth = 0;
        while (i < tokens.Count)
        {
            var t = tokens[i];
            if (t.Type == TokenType.OpenBrace) braceDepth++;
            if (t.Type == TokenType.CloseBrace)
            {
                braceDepth--;
                if (braceDepth < 0) break; // 超出了类体
            }
            if (t.Type == TokenType.Semicolon && braceDepth == 0)
            {
                semicolonPos = i;
                break;
            }
            i++;
        }

        if (semicolonPos < 0) return startIndex + 1;

        // 提取声明片段 [start, semicolonPos) 中的 Token
        var declTokens = new List<Token>();
        for (int j = start; j < semicolonPos; j++)
            declTokens.Add(tokens[j]);

        // 如果是空的声明
        if (declTokens.Count == 0) return semicolonPos + 1;

        // 解析属性声明
        // 从后往前找变量名：跳过括号匹配，找到第一个 Word
        string propName = "";
        int nameIndex = -1;
        for (int j = declTokens.Count - 1; j >= 0; j--)
        {
            if (declTokens[j].Type == TokenType.Word)
            {
                // 跳过 "const" 等关键字 — 它们不会是变量名
                if (IsTypeQualifier(declTokens[j].Value))
                    continue;
                propName = declTokens[j].Value;
                nameIndex = j;
                break;
            }
        }

        if (string.IsNullOrEmpty(propName))
            return semicolonPos + 1;

        // 提取类型部分：[0, nameIndex)
        var typeTokens = new List<Token>();
        for (int j = 0; j < nameIndex; j++)
            typeTokens.Add(declTokens[j]);

        string propType = ReconstructType(typeTokens);

        // 检查是否为 static
        bool isStatic = false;
        var staticIdx = typeTokens.FindIndex(t => t.Type == TokenType.Word && t.Value == "static");
        if (staticIdx >= 0)
        {
            isStatic = true;
            typeTokens.RemoveAt(staticIdx);
            propType = ReconstructType(typeTokens);
        }

        if (_currentClass.HasValue)
        {
            var classInfo = _currentClass.Value;
            // 这里暂时不添加到 classInfo，因为 ClassStructure 当前没有 Properties 列表
            // 后续扩展 ClassStructure 时会添加
            Console.WriteLine($"    [属性] {propType} {propName} ({_currentAccessModifier})");
        }

        return semicolonPos + 1;
    }

    // ──────────────────────────────────────────────
    // 函数解析
    // ──────────────────────────────────────────────
    private int ParseFunction(List<Token> tokens, int startIndex)
    {
        int i = startIndex;
        if (i >= tokens.Count) return i;

        // 收集 Token 直到遇到 {（函数体开始）或 ;（纯声明）
        int start = i;
        int braceDepth = 0;
        int angleDepth = 0;
        int parenDepth = 0;
        int endPos = -1;

        while (i < tokens.Count)
        {
            var t = tokens[i];
            if (t.Type == TokenType.OpenParen) parenDepth++;
            if (t.Type == TokenType.CloseParen) parenDepth--;
            if (t.Type == TokenType.OpenBrace)
            {
                if (parenDepth == 0) // 只有不在参数列表里的 { 才是函数体
                {
                    endPos = i;
                    break;
                }
                braceDepth++;
            }
            if (t.Type == TokenType.CloseBrace && braceDepth > 0) braceDepth--;
            if (t.Type == TokenType.Semicolon && parenDepth == 0 && braceDepth == 0)
            {
                endPos = i;
                break;
            }
            i++;
        }

        if (endPos < 0) return startIndex + 1;

        // 提取声明部分 Token [start, endPos)
        var declTokens = new List<Token>();
        for (int j = start; j < endPos; j++)
            declTokens.Add(tokens[j]);

        if (declTokens.Count == 0) return endPos + 1;

        // 解析函数声明：
        // 找到第一个 '(' 之前的内容作为返回类型 + 函数名
        // 括号内的作为参数列表
        int parenIdx = declTokens.FindIndex(t => t.Type == TokenType.OpenParen);
        if (parenIdx < 0) return endPos + 1;

        // 参数列表部分
        var paramTokens = new List<Token>();
        int closeParenIdx = FindMatchingCloseParen(declTokens, parenIdx);
        for (int j = parenIdx + 1; j < closeParenIdx; j++)
            paramTokens.Add(declTokens[j]);

        // 函数名前部分（返回类型 + 函数名）
        // 从 parenIdx 往前找第一个 Word 作为函数名
        string funcName = "";
        int funcNameIdx = -1;
        for (int j = parenIdx - 1; j >= 0; j--)
        {
            if (declTokens[j].Type == TokenType.Word)
            {
                funcName = declTokens[j].Value;
                funcNameIdx = j;
                break;
            }
        }

        if (string.IsNullOrEmpty(funcName))
            return endPos + 1;

        // 返回类型：[0, funcNameIdx)
        var returnTypeTokens = new List<Token>();
        for (int j = 0; j < funcNameIdx; j++)
            returnTypeTokens.Add(declTokens[j]);

        string returnType = ReconstructType(returnTypeTokens);

        // 检查是否有 const 修饰（在参数列表括号后）
        bool isConst = false;
        if (closeParenIdx + 1 < declTokens.Count)
        {
            for (int j = closeParenIdx + 1; j < declTokens.Count; j++)
            {
                if (declTokens[j].Type == TokenType.Word && declTokens[j].Value == "const")
                {
                    isConst = true;
                    break;
                }
                if (declTokens[j].Type == TokenType.Semicolon) break;
            }
        }

        // 解析参数
        var paramList = ParseParameterList(paramTokens);

        if (_currentClass.HasValue)
        {
            Console.WriteLine($"    [函数] {returnType} {funcName}({string.Join(", ", paramList.Select(p => string.Join(" ", p.ParamType)))}) ({_currentAccessModifier}){(isConst ? " const" : "")}");
        }

        // 如果有函数体（{...}），跳过它
        if (endPos < tokens.Count && tokens[endPos].Type == TokenType.OpenBrace)
        {
            i = SkipBalancedBlock(tokens, endPos);
        }
        else
        {
            i = endPos + 1; // 跳过分号
        }

        return i;
    }

    // ──────────────────────────────────────────────
    // 枚举解析
    // ──────────────────────────────────────────────
    private int ParseEnum(List<Token> tokens, int startIndex)
    {
        int i = startIndex;
        if (i >= tokens.Count) return i;

        // 期望: enum [class/struct] EnumName [:
        //   UnderlyingType] {
        if (tokens[i].Type != TokenType.Word || tokens[i].Value != "enum")
            return i;

        i++; // 跳过 enum

        // 跳过 enum class / enum struct
        if (i < tokens.Count && tokens[i].Type == TokenType.Word &&
            (tokens[i].Value == "class" || tokens[i].Value == "struct"))
            i++;

        // 获取枚举名
        if (i >= tokens.Count || tokens[i].Type != TokenType.Word)
            return i;
        string enumName = tokens[i].Value;
        i++;

        var enumInfo = new AbstractStructures.EnumStructure
        {
            NamespaceName = _namespaceName,
            EnumName = enumName,
            EnumValueList = new List<string>()
        };

        // 跳过可选的 : UnderlyingType
        if (i < tokens.Count && tokens[i].Type == TokenType.Colon)
        {
            i++;
            while (i < tokens.Count && tokens[i].Type == TokenType.Word)
                i++;
        }

        // 期望 {
        if (i >= tokens.Count || tokens[i].Type != TokenType.OpenBrace)
            return i;

        i++; // 跳过 {

        // 解析枚举值列表
        while (i < tokens.Count)
        {
            var t = tokens[i];
            if (t.Type == TokenType.CloseBrace) { i++; break; }
            if (t.Type == TokenType.Comma) { i++; continue; }
            if (t.Type == TokenType.Word)
            {
                enumInfo.EnumValueList.Add(t.Value);
                i++;
                continue;
            }
            if (t.Type == TokenType.Assign)
            {
                // 跳过 = value
                i++;
                while (i < tokens.Count && tokens[i].Type != TokenType.Comma &&
                       tokens[i].Type != TokenType.CloseBrace)
                    i++;
                continue;
            }
            i++;
        }

        // 跳过尾部分号（enum X {};）
        if (i < tokens.Count && tokens[i].Type == TokenType.Semicolon)
            i++;

        _enums.Add(enumInfo);
        Console.WriteLine($"  [枚举] {enumName} ({enumInfo.EnumValueList.Count} 个值)");

        return i;
    }

    // ──────────────────────────────────────────────
    // 工具方法
    // ──────────────────────────────────────────────

    /// <summary>
    /// 将类型 Token 列表重新组合为类型字符串
    /// </summary>
    private static string ReconstructType(List<Token> tokens)
    {
        if (tokens.Count == 0) return "void";
        var sb = new StringBuilder();
        for (int i = 0; i < tokens.Count; i++)
        {
            var t = tokens[i];
            string val = t.Value;

            // 指针/引用符号紧贴前一个内容
            if (t.Type == TokenType.Star || t.Type == TokenType.Amp || t.Type == TokenType.AmpAmp)
            {
                // 如果是 const char* 这种情况，星号前应有空格
                sb.Append(val);
            }
            else
            {
                if (sb.Length > 0) sb.Append(' ');
                sb.Append(val);
            }
        }
        return sb.ToString().Trim();
    }

    /// <summary>
    /// 判断是否为类型限定关键字（不是变量名）
    /// </summary>
    private static bool IsTypeQualifier(string word)
    {
        return word switch
        {
            "const" => true,
            "volatile" => true,
            "mutable" => true,
            "static" => true,
            "extern" => true,
            "virtual" => true,
            "unsigned" => true,
            "signed" => true,
            "long" => true,
            "short" => true,
            "auto" => true,
            "register" => true,
            "inline" => true,
            "constexpr" => true,
            "consteval" => true,
            "constinit" => true,
            _ => false
        };
    }

    /// <summary>
    /// 解析参数列表 Token 为 ParamInfo 列表
    /// </summary>
    private static List<AbstractStructures.ParamInfo> ParseParameterList(List<Token> tokens)
    {
        var result = new List<AbstractStructures.ParamInfo>();
        if (tokens.Count == 0) return result;

        // 按逗号分割
        var currentParam = new List<Token>();
        int depth = 0; // 括号嵌套深度

        for (int i = 0; i < tokens.Count; i++)
        {
            var t = tokens[i];
            if (t.Type == TokenType.OpenParen) { depth++; currentParam.Add(t); continue; }
            if (t.Type == TokenType.CloseParen) { depth--; currentParam.Add(t); continue; }
            if (t.Type == TokenType.Comma && depth == 0)
            {
                // 完成一个参数
                var paramInfo = ExtractSingleParam(currentParam);
                if (paramInfo.HasValue) result.Add(paramInfo.Value);
                currentParam.Clear();
                continue;
            }
            currentParam.Add(t);
        }

        // 最后一个参数
        if (currentParam.Count > 0)
        {
            var paramInfo = ExtractSingleParam(currentParam);
            if (paramInfo.HasValue) result.Add(paramInfo.Value);
        }

        return result;
    }

    /// <summary>
    /// 从参数 Token 列表中提取单个参数信息
    /// </summary>
    private static AbstractStructures.ParamInfo? ExtractSingleParam(List<Token> tokens)
    {
        if (tokens.Count == 0) return null;

        // 处理 void（无参数）
        if (tokens.Count == 1 && tokens[0].Type == TokenType.Word && tokens[0].Value == "void")
            return null;

        // 去掉默认值部分（= ...）
        int assignIdx = tokens.FindIndex(t => t.Type == TokenType.Assign);
        List<Token> declTokens;
        if (assignIdx >= 0)
            declTokens = tokens.GetRange(0, assignIdx);
        else
            declTokens = new List<Token>(tokens);

        if (declTokens.Count == 0) return null;

        // 从后往前找参数名（跳过括号匹配）
        string paramName = "";
        int nameIdx = -1;
        for (int j = declTokens.Count - 1; j >= 0; j--)
        {
            if (declTokens[j].Type == TokenType.Word && !IsTypeQualifier(declTokens[j].Value))
            {
                paramName = declTokens[j].Value;
                nameIdx = j;
                break;
            }
        }

        List<Token> typeTokens;
        if (nameIdx >= 0)
            typeTokens = declTokens.GetRange(0, nameIdx);
        else
            typeTokens = new List<Token>(declTokens);

        // 清理类型部分末尾的 const
        while (typeTokens.Count > 0 && typeTokens[typeTokens.Count - 1].Type == TokenType.Word &&
               typeTokens[typeTokens.Count - 1].Value == "const")
            typeTokens.RemoveAt(typeTokens.Count - 1);

        string paramType = ReconstructType(typeTokens);

        return new AbstractStructures.ParamInfo
        {
            ParamType = new List<string> { paramType }
        };
    }

    /// <summary>
    /// 查找匹配的右括号
    /// </summary>
    private static int FindMatchingCloseParen(List<Token> tokens, int openIdx)
    {
        int depth = 1;
        for (int i = openIdx + 1; i < tokens.Count; i++)
        {
            if (tokens[i].Type == TokenType.OpenParen) depth++;
            if (tokens[i].Type == TokenType.CloseParen) { depth--; if (depth == 0) return i; }
        }
        return tokens.Count - 1;
    }

    /// <summary>
    /// 跳过宏调用（如 SCLASS() 或 GENERATED_BODY()）
    /// </summary>
    private static int SkipMacroCall(List<Token> tokens, int startIndex)
    {
        int i = startIndex;
        if (i >= tokens.Count) return i;
        // 跳过宏名
        if (tokens[i].Type == TokenType.Word) i++;
        // 跳过 (...)
        if (i < tokens.Count && tokens[i].Type == TokenType.OpenParen)
            i = SkipBalancedParen(tokens, i);
        return i;
    }

    /// <summary>
    /// 跳过平衡的括号 (...)
    /// </summary>
    private static int SkipBalancedParen(List<Token> tokens, int startIndex)
    {
        int i = startIndex;
        if (i >= tokens.Count || tokens[i].Type != TokenType.OpenParen)
            return i;
        int depth = 1;
        i++;
        while (i < tokens.Count && depth > 0)
        {
            if (tokens[i].Type == TokenType.OpenParen) depth++;
            if (tokens[i].Type == TokenType.CloseParen) depth--;
            i++;
        }
        return i;
    }

    /// <summary>
    /// 跳过平衡的 {} 块
    /// </summary>
    private static int SkipBalancedBlock(List<Token> tokens, int startIndex)
    {
        int i = startIndex;
        if (i >= tokens.Count || tokens[i].Type != TokenType.OpenBrace)
            return i;
        int depth = 1;
        i++;
        while (i < tokens.Count && depth > 0)
        {
            if (tokens[i].Type == TokenType.OpenBrace) depth++;
            if (tokens[i].Type == TokenType.CloseBrace) depth--;
            i++;
        }
        return i;
    }

    /// <summary>
    /// 跳过到分号
    /// </summary>
    private static int SkipToSemicolon(List<Token> tokens, int startIndex)
    {
        int i = startIndex;
        while (i < tokens.Count && tokens[i].Type != TokenType.Semicolon)
            i++;
        if (i < tokens.Count) i++;
        return i;
    }

    /// <summary>
    /// 跳过到行尾（预处理器行通常以换行结尾，Token 流中表现为遇到下一个 # 或 { 或 }）
    /// </summary>
    private static int SkipLine(List<Token> tokens, int startIndex)
    {
        int i = startIndex;
        while (i < tokens.Count)
        {
            var t = tokens[i];
            // 遇到下一个 # 开头或 {} 块边界就停止
            if (t.Type == TokenType.Word && t.Value == "#") break;
            if (t.Type == TokenType.OpenBrace) break;
            if (t.Type == TokenType.CloseBrace) break;
            i++;
        }
        return i;
    }

    /// <summary>
    /// 跳过预处理器条件块 (#if / #ifdef / #ifndef ... #endif)
    /// </summary>
    private static int SkipPreprocessorBlock(List<Token> tokens, int startIndex)
    {
        int i = startIndex;
        while (i < tokens.Count)
        {
            // 遇到 #endif 结束
            if (tokens[i].Type == TokenType.Word && tokens[i].Value == "#")
            {
                int next = i + 1;
                if (next < tokens.Count && tokens[next].Type == TokenType.Word)
                {
                    string d = tokens[next].Value;
                    if (d == "endif")
                    {
                        i = next + 1;
                        break;
                    }
                    if (d == "if" || d == "ifdef" || d == "ifndef")
                    {
                        // 嵌套
                        i = SkipPreprocessorBlock(tokens, i);
                        continue;
                    }
                    if (d == "else" || d == "elif")
                    {
                        // 跳过 else/elif 分支
                        i = SkipLine(tokens, i);
                        continue;
                    }
                }
            }
            i++;
        }
        return i;
    }

    /// <summary>
    /// 跳过 <> 括号（模板参数）
    /// </summary>
    private static int SkipBrackets(List<Token> tokens, int startIndex)
    {
        int i = startIndex;
        if (i >= tokens.Count || tokens[i].Type != TokenType.OpenBracket)
            return i;
        int depth = 1;
        i++;
        while (i < tokens.Count && depth > 0)
        {
            if (tokens[i].Type == TokenType.OpenBracket) depth++;
            if (tokens[i].Type == TokenType.CloseBracket) depth--;
            i++;
        }
        return i;
    }

    /// <summary>
    /// 跳过函数声明（没有 SFUNCTION 标记的，比如构造函数）
    /// </summary>
    private static int SkipFunctionDeclaration(List<Token> tokens, int startIndex)
    {
        int i = startIndex;
        int parenDepth = 0;
        while (i < tokens.Count)
        {
            var t = tokens[i];
            if (t.Type == TokenType.OpenParen) parenDepth++;
            if (t.Type == TokenType.CloseParen) parenDepth--;
            if (t.Type == TokenType.OpenBrace && parenDepth == 0)
            {
                return SkipBalancedBlock(tokens, i);
            }
            if (t.Type == TokenType.Semicolon && parenDepth == 0)
            {
                return i + 1;
            }
            i++;
        }
        return i;
    }

    /// <summary>
    /// 跳过 template 声明
    /// </summary>
    private static int SkipTemplateDeclaration(List<Token> tokens, int startIndex)
    {
        int i = startIndex;
        // 跳过 template<...>
        if (i < tokens.Count && tokens[i].Type == TokenType.Word && tokens[i].Value == "template")
            i++;
        if (i < tokens.Count && tokens[i].Type == TokenType.OpenBracket)
            i = SkipBrackets(tokens, i);
        // 后面的函数/类声明由调用者处理
        return i;
    }

    /// <summary>
    /// 输出解析结果
    /// </summary>
    public void PrintResults()
    {
        Console.WriteLine($"\n===== {_fileName} =====");
        Console.WriteLine($"  命名空间: {_namespaceName}");
        Console.WriteLine($"  类: {_classes.Count}, 枚举: {_enums.Count}");

        foreach (var cls in _classes)
        {
            Console.WriteLine($"\n  [类] {cls.ClassName}");
            if (cls.InheritanceInfo.InheritanceNameList.Count > 0)
            {
                Console.WriteLine($"    继承: {string.Join(", ", cls.InheritanceInfo.InheritanceNameList)}");
            }
        }

        foreach (var enu in _enums)
        {
            Console.WriteLine($"  [枚举] {enu.EnumName}: {string.Join(", ", enu.EnumValueList)}");
        }
    }
}
