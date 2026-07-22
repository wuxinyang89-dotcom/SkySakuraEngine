using ReflectionParser.Public;

namespace ReflectionParser;

public class ReflectionFileGenerator
{
    private string _headerText;
    private string _filePath;

    public ReflectionFileGenerator(int index)
    {
        _filePath = PublicVariables.ReflectionHeaders[index].HeaderPath;
        _headerText = PublicVariables.ReflectionHeaders[index].HeaderFileText;

    }
}