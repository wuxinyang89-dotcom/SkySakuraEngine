namespace ReflectionParser.Public;

public struct HeaderInfo
{
    public string HeaderPath;
    public string HeaderFileText;
}

public static class PublicVariables
{
    public static List<HeaderInfo> ReflectionHeaders { get; set; } = new();
    
}