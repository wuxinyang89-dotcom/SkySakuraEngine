using System.Diagnostics;

namespace ReflectionParser;

class Program
{
    static void Main(string[] args)
    {
        Console.WriteLine("╔══════════════════════════════════════════════════╗");
        Console.WriteLine("║       SkySakura Reflection Parser v0.1          ║");
        Console.WriteLine("╚══════════════════════════════════════════════════╝");
        Console.WriteLine();

        try
        {
            // ──────────────────────────────────────────────
            // Step 1: 确定解决方案根目录
            // ──────────────────────────────────────────────
            string solutionRoot = ResolveSolutionRoot(args);
            Console.WriteLine($"[1/4] 解决方案根目录: {solutionRoot}");
            Console.WriteLine();

            // ──────────────────────────────────────────────
            // Step 2: 扫描头文件
            // ──────────────────────────────────────────────
            Console.WriteLine("[2/4] 正在扫描头文件...");
            var stopwatch = Stopwatch.StartNew();

            var scanner = new HeaderScanner();
            scanner.AddProjectDirectoriesFromRoot(solutionRoot);
            scanner.Scan();

            stopwatch.Stop();
            Console.WriteLine($"  扫描完成，耗时 {stopwatch.ElapsedMilliseconds} ms");
            Console.WriteLine();

            // ──────────────────────────────────────────────
            // Step 3: 输出扫描结果
            // ──────────────────────────────────────────────
            Console.WriteLine("[3/4] 扫描结果:");
            scanner.PrintSummary();
            Console.WriteLine();

            // ──────────────────────────────────────────────
            // Step 4: 后续反射代码生成（待实现）
            // ──────────────────────────────────────────────
            if (scanner.ReflectionHeaders.Count == 0)
            {
                Console.WriteLine("[4/4] 未发现需要处理的反射头文件，退出。");
            }
            else
            {
                Console.WriteLine($"[4/4] 准备为 {scanner.ReflectionHeaders.Count} 个文件生成反射代码...");
                Console.WriteLine("  (代码生成器尚未实现，后续版本补充)");

                for (int i = 0; i < scanner.ReflectionHeaders.Count; i++)
                {
                    ReflectionFileGenerator generator = new(i);
                }
            }
        }
        catch (Exception ex)
        {
            var oldColor = Console.ForegroundColor;
            Console.ForegroundColor = ConsoleColor.Red;
            Console.Error.WriteLine($"发生错误: {ex.Message}");
            Console.ForegroundColor = oldColor;
        }
        finally
        {
            Console.WriteLine();
            Console.WriteLine("完成。");
        }
        
    }

    /// <summary>
    /// 解析解决方案根目录：优先从命令行参数获取，否则自动向上查找 .sln 文件
    /// </summary>
    private static string ResolveSolutionRoot(string[] args)
    {
        // 优先使用命令行参数
        if (args.Length > 0)
        {
            if (!Directory.Exists(args[0]))
                throw new DirectoryNotFoundException($"指定的目录不存在: {args[0]}");
            return Path.GetFullPath(args[0]);
        }

        // 自动从当前目录向上查找包含 .sln 的目录
        string? current = Directory.GetCurrentDirectory();
        while (current != null)
        {
            if (Directory.GetFiles(current, "*.sln").Length > 0)
            {
                return current;
            }
            current = Directory.GetParent(current)?.FullName;
        }

        throw new FileNotFoundException("未找到解决方案文件 (.sln)，请从包含 .sln 的目录运行或通过命令行参数指定路径。");
    }
}
