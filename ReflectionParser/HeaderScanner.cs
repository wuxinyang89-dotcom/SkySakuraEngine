using ReflectionParser.Public;

namespace ReflectionParser;

/// <summary>
/// 头文件扫描器：从指定的项目目录中递归搜索所有 .h 头文件，
/// 并筛选出包含反射宏 (SCLASS/SPROPERTY/SFUNCTION) 的文件。
/// </summary>
public class HeaderScanner
{
    // 需要扫描的项目目录列表
    private readonly List<string> _searchDirectories = new();

    // 反射宏关键字
    private static readonly string[] ReflectionMarkers = { "SCLASS()", "SPROPERTY()", "SFUNCTION()" };

    /// <summary>
    /// 所有扫描到的头文件路径
    /// </summary>
    public List<string> AllHeaders { get; private set; } = new();

    /// <summary>
    /// 包含反射宏的头文件路径
    /// </summary>
    public List<string> ReflectionHeaders { get; private set; } = new();

    /// <summary>
    /// 添加一个需要扫描的目录
    /// </summary>
    public void AddSearchDirectory(string directory)
    {
        if (Directory.Exists(directory))
        {
            _searchDirectories.Add(directory);
        }
        else
        {
            Console.WriteLine($"[警告] 目录不存在，已跳过: {directory}");
        }
    }

    /// <summary>
    /// 根据解决方案根目录自动添加所有项目目录
    /// </summary>
    public void AddProjectDirectoriesFromRoot(string solutionRoot)
    {
        // 扫描引擎核心目录
        string engineDir = Path.Combine(solutionRoot, "SkySakuraEngine");
        if (Directory.Exists(engineDir))
            _searchDirectories.Add(engineDir);

        // 扫描编辑器目录
        string editorDir = Path.Combine(solutionRoot, "Editor");
        if (Directory.Exists(editorDir))
            _searchDirectories.Add(editorDir);

        // 扫描用户项目目录
        string userDir = Path.Combine(solutionRoot, "User");
        if (Directory.Exists(userDir))
            _searchDirectories.Add(userDir);
    }

    /// <summary>
    /// 执行扫描：遍历所有注册目录，收集头文件
    /// </summary>
    public void Scan()
    {
        AllHeaders.Clear();
        ReflectionHeaders.Clear();

        foreach (var dir in _searchDirectories)
        {
            var headers = Directory.GetFiles(dir, "*.h", SearchOption.AllDirectories);
            AllHeaders.AddRange(headers);
        }

        // 从所有头文件中筛选出包含反射宏的文件
        foreach (var header in AllHeaders)
        {
            // 跳过 .generated.h 文件（这些是生成的输出）
            if (header.EndsWith(".generated.h", StringComparison.OrdinalIgnoreCase))
                continue;

            // 跳过宏定义文件本身（它们定义宏但不是反射目标）
            string fileName = Path.GetFileName(header);
            if (fileName.Equals("ReflectionMacros.h", StringComparison.OrdinalIgnoreCase) ||
                fileName.Equals("ReflectionFeatures.h", StringComparison.OrdinalIgnoreCase))
                continue;

            if (ContainsReflectionMarkers(header))
            {
                ReflectionHeaders.Add(header);
            }
        }
    }

    /// <summary>
    /// 检查文件中是否包含反射宏标记
    /// </summary>
    private static bool ContainsReflectionMarkers(string filePath)
    {
        try
        {
            string content = File.ReadAllText(filePath);
            foreach (var marker in ReflectionMarkers)
            {
                if (content.Contains(marker))
                {
                    PublicVariables.ReflectionHeaders.Add(new HeaderInfo()
                    {
                        HeaderFileText = filePath,
                        HeaderPath = content
                    });
                    return true;
                }
            }
        }
        catch (Exception ex)
        {
            throw new FileLoadException($"[错误] 读取文件失败: {filePath} - {ex.Message}");
        }

        return false;
    }

    /// <summary>
    /// 打印扫描结果摘要
    /// </summary>
    public void PrintSummary()
    {
        Console.WriteLine($"  扫描目录数: {_searchDirectories.Count}");
        Console.WriteLine($"  发现头文件: {AllHeaders.Count} 个");
        Console.WriteLine($"  含反射宏的头文件: {ReflectionHeaders.Count} 个");
        Console.WriteLine();

        if (ReflectionHeaders.Count > 0)
        {
            Console.WriteLine("  需要生成反射代码的文件:");
            foreach (var header in ReflectionHeaders)
            {
                Console.WriteLine($"    -> {header}");
            }
        }
    }
}
