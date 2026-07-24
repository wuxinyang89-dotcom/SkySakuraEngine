namespace ReflectionParser;

/// <summary>
/// Token 类型枚举
/// </summary>
public enum TokenType
{
    Word,           // 普通单词（标识符、关键字、数字字面量等）
    OpenParen,      // (
    CloseParen,     // )
    OpenBrace,      // {
    CloseBrace,     // }
    OpenBracket,    // < （泛型、模板参数）
    CloseBracket,   // > 
    Semicolon,      // ;
    Colon,          // :
    DoubleColon,    // ::
    Comma,          // ,
    Star,           // *
    Amp,            // &
    AmpAmp,         // &&
    Tilde,          // ~
    Assign,         // =
    Comment,        // 注释（会被跳过）
    StringLiteral,  // 字符串字面量
    EndOfFile,      // 文件结尾
}

/// <summary>
/// Token 结构体
/// </summary>
public struct Token
{
    public TokenType Type;
    public string Value;      // 单词的文本内容（仅 Word 类型有实际值，符号类也可有值用于调试）
    public int Position;      // 在原始文本中的位置（用于错误报告）
}

/// <summary>
/// C++ 头文件词法分析器。
/// 只产生扁平 Token 流，不做语法分析。
/// 注释会被跳过。
/// </summary>
public class Tokenizer
{
    private readonly string _text;
    private int _pos;
    private readonly int _length;

    public Tokenizer(string text)
    {
        _text = text;
        _length = text.Length;
        _pos = 0;
    }

    /// <summary>
    /// 将整个文本 Token 化，返回 Token 列表（注释已被跳过）
    /// </summary>
    public List<Token> Tokenize()
    {
        var tokens = new List<Token>();
        while (_pos < _length)
        {
            var token = NextToken();
            if (token.Type == TokenType.Comment)
                continue; // 跳过注释
            if (token.Type == TokenType.EndOfFile)
                break;
            tokens.Add(token);
        }
        tokens.Add(new Token { Type = TokenType.EndOfFile, Position = _pos });
        return tokens;
    }

    /// <summary>
    /// 读取下一个 Token
    /// </summary>
    private Token NextToken()
    {
        SkipWhitespace();
        if (_pos >= _length)
            return new Token { Type = TokenType.EndOfFile, Position = _pos };

        int start = _pos;
        char c = _text[_pos];

        // 注释处理
        if (c == '/' && _pos + 1 < _length)
        {
            char next = _text[_pos + 1];
            if (next == '/')
            {
                // 单行注释 //
                while (_pos < _length && _text[_pos] != '\n')
                    _pos++;
                return new Token { Type = TokenType.Comment, Position = start };
            }
            if (next == '*')
            {
                // 多行注释 /* ... */
                _pos += 2;
                while (_pos + 1 < _length && !(_text[_pos] == '*' && _text[_pos + 1] == '/'))
                    _pos++;
                if (_pos + 1 < _length)
                    _pos += 2; // 跳过 */
                return new Token { Type = TokenType.Comment, Position = start };
            }
        }

        // 字符串字面量 "..."
        if (c == '"')
        {
            _pos++;
            while (_pos < _length)
            {
                if (_text[_pos] == '\\') { _pos += 2; continue; }
                if (_text[_pos] == '"') { _pos++; break; }
                _pos++;
            }
            return new Token
            {
                Type = TokenType.StringLiteral,
                Value = _text.Substring(start, _pos - start),
                Position = start
            };
        }

        // 字符字面量 '...'
        if (c == '\'')
        {
            _pos++;
            while (_pos < _length)
            {
                if (_text[_pos] == '\\') { _pos += 2; continue; }
                if (_text[_pos] == '\'') { _pos++; break; }
                _pos++;
            }
            return new Token
            {
                Type = TokenType.StringLiteral,
                Value = _text.Substring(start, _pos - start),
                Position = start
            };
        }

        // 双字符符号
        if (_pos + 1 < _length)
        {
            string twoChar = _text.Substring(_pos, 2);
            switch (twoChar)
            {
                case "::": _pos += 2; return Sym(TokenType.DoubleColon, "::", start);
                case "&&": _pos += 2; return Sym(TokenType.AmpAmp, "&&", start);
                case "/*": break; // 已在上面处理
                case "//": break; // 已在上面处理
            }
        }

        // 单字符符号
        switch (c)
        {
            case '(': _pos++; return Sym(TokenType.OpenParen, "(", start);
            case ')': _pos++; return Sym(TokenType.CloseParen, ")", start);
            case '{': _pos++; return Sym(TokenType.OpenBrace, "{", start);
            case '}': _pos++; return Sym(TokenType.CloseBrace, "}", start);
            case ';': _pos++; return Sym(TokenType.Semicolon, ";", start);
            case ',': _pos++; return Sym(TokenType.Comma, ",", start);
            case '*': _pos++; return Sym(TokenType.Star, "*", start);
            case '~': _pos++; return Sym(TokenType.Tilde, "~", start);
            case '=': _pos++; return Sym(TokenType.Assign, "=", start);
            case ':':
                // 已经在双字符中处理了 ::，这里处理单独冒号（如 public:）
                _pos++;
                return Sym(TokenType.Colon, ":", start);
            case '<':
                // 注意：< 和 > 可能出现在模板、运算符、头文件包含中
                // 我们简单处理为符号，在语法层做括号匹配
                _pos++;
                return Sym(TokenType.OpenBracket, "<", start);
            case '>':
                _pos++;
                return Sym(TokenType.CloseBracket, ">", start);
            case '&':
                // 单 & 是引用
                _pos++;
                return Sym(TokenType.Amp, "&", start);
        }

        // 数字字面量：以数字开头
        if (char.IsDigit(c) || (c == '.' && _pos + 1 < _length && char.IsDigit(_text[_pos + 1])))
        {
            // 读取数字：包括十六进制 0x、浮点数
            if (c == '0' && _pos + 1 < _length && (_text[_pos + 1] == 'x' || _text[_pos + 1] == 'X'))
            {
                _pos += 2;
                while (_pos < _length && IsHexDigit(_text[_pos])) _pos++;
            }
            else
            {
                _pos++;
                while (_pos < _length)
                {
                    c = _text[_pos];
                    if (char.IsDigit(c) || c == '.' || c == 'x' || c == 'X' || c == 'p' || c == 'P')
                    {
                        if ((c == 'x' || c == 'X') && _text[_pos - 1] != '0') break;
                        _pos++;
                    }
                    else if (c == 'e' || c == 'E' || c == 'f' || c == 'F' || c == 'u' || c == 'U' ||
                             c == 'l' || c == 'L')
                    {
                        _pos++;
                        // 跳过可能的符号
                        if (_pos < _length && (_text[_pos] == '+' || _text[_pos] == '-'))
                            _pos++;
                        continue;
                    }
                    else break;
                }
            }
            return Word(_text.Substring(start, _pos - start), start);
        }

        // 普通单词：字母、数字、下划线、#（预处理器）
        if (char.IsLetter(c) || c == '_' || c == '#')
        {
            _pos++;
            while (_pos < _length)
            {
                c = _text[_pos];
                if (char.IsLetterOrDigit(c) || c == '_')
                    _pos++;
                else
                    break;
            }
            return Word(_text.Substring(start, _pos - start), start);
        }

        // 无法识别的字符：跳过并继续（如 @、$ 等，C++ 中不常见）
        _pos++;
        return Word(c.ToString(), start);
    }

    private void SkipWhitespace()
    {
        while (_pos < _length && char.IsWhiteSpace(_text[_pos]))
            _pos++;
    }

    private static bool IsHexDigit(char c)
    {
        return char.IsDigit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
    }

    private static Token Sym(TokenType type, string value, int pos)
    {
        return new Token { Type = type, Value = value, Position = pos };
    }

    private static Token Word(string value, int pos)
    {
        return new Token { Type = TokenType.Word, Value = value, Position = pos };
    }
}
