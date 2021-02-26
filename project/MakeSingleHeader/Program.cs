using System.IO;
using System.Text;

namespace MakeSingleHeader
{
internal static class Program
{
    private static void Main(string[] args)
    {
        var linqFilePath = args.Length == 0 ? "../../../../CppLinq/Linq.h" : args[0];
        linqFilePath = Path.GetFullPath(linqFilePath);
        var linqDir = Path.GetDirectoryName(linqFilePath);
        var rawFile = File.ReadAllLines(linqFilePath);

        var builder = new StringBuilder();
        foreach (var line in rawFile)
        {
            const string startStr = "#include \"";
            // linq file
            if (line.StartsWith(startStr))
            {
                var headerName    = line.Substring(startStr.Length, line.Length - startStr.Length - 1);
                var headerRawFile = File.ReadAllLines(linqDir                   + "/"             + headerName);
                foreach (var headerLine in headerRawFile)
                {
                    if (!headerLine.StartsWith(startStr) && !headerLine.StartsWith("#pragma"))
                    {
                        builder.AppendLine(headerLine);
                    }
                }
            }
            else
            {
                builder.AppendLine(line);
            }
        }
        File.WriteAllText(linqDir + "/Linq.hpp", builder.ToString());
    }
}
}