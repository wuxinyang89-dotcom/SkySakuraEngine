namespace ReflectionParser.Public;

public enum PackageType
{
    Public, Private, Protected
}

public class AbstractStructures
{
    
    
    public struct ClassStructure
    {
        public string NamespaceName;
        public string ClassName;
        
        public InheritanceInfo InheritanceInfo;

        public List<PropertyStructure> Properties;
        public List<FunctionStructure> Functions;

    }

    public struct InheritanceInfo
    {
        public List<string> InheritanceNameList;
    }
    
    public struct EnumStructure
    {
        public string NamespaceName;
        public string EnumName;
        public List<string> EnumValueList;
    }
    
    public struct FunctionStructure
    {
        public string FunctionName;
        public List<string> ReturnTypes;
        public List<ParamInfo> ParameterList;
        public PackageType PackageType;
    }
    
    public struct ParamInfo
    {
        public List<string> ParamType;
    }
    
    public struct PropertyStructure
    {
        public bool IsStatic;
        public string PropertyName;
        public string PropertyType;
        public PackageType PackageType;
    }
}