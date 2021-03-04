using System;
using System.Collections.Generic;
using System.IO;
using System.IO.Compression;
using System.Linq;
using System.Text;

namespace Packaging
{
internal static class Program
{
    private static void CopyDirectory(string sourceDirName, string destDirName)
    {
        if (!Directory.Exists(destDirName))
        {
            Directory.CreateDirectory(destDirName);
            File.SetAttributes(destDirName, File.GetAttributes(sourceDirName));
        }

        if (destDirName.Last() != Path.DirectorySeparatorChar)
            destDirName += Path.DirectorySeparatorChar;

        var files = Directory.GetFiles(sourceDirName);
        foreach (var file in files)
            File.Copy(file, destDirName + Path.GetFileName(file), true);

        var dirs = Directory.GetDirectories(sourceDirName);
        foreach (var dir in dirs)
            CopyDirectory(dir, destDirName + Path.GetFileName(dir));
    }

    private static void MakeRelease(string projectDir)
    {
        var linqDir = projectDir + "Linq/";
        var rawFile = File.ReadAllLines(linqDir + "Linq.h");

        var          builder       = new StringBuilder();
        var          defineBuilder = new StringBuilder();
        var          includeList   = new List<string>();
        const string include       = "#include \"";
        const string nameSpace     = "namespace linq {";
        var          nestCnt       = -1;
        foreach (var line in rawFile)
        {
            if (line.StartsWith("#pragma")) continue;
            // linq file
            if (line.StartsWith(include))
            {
                var headerName    = line.Substring(include.Length, line.Length - include.Length - 1);
                var headerRawFile = File.ReadAllLines(linqDir                  + headerName);
                foreach (var headerLine in headerRawFile)
                {
                    if (headerLine.StartsWith("#pragma") || headerLine.StartsWith(include)) continue;

                    if (headerLine.StartsWith("#define"))
                    {
                        defineBuilder.AppendLine(headerLine);
                        continue;
                    }

                    if (headerLine.StartsWith("#include <"))
                    {
                        includeList.Add(headerLine);
                        continue;
                    }

                    if (nestCnt > -1 && headerLine.Any(c => c == '{')) nestCnt += headerLine.Count(c => c == '{');

                    if (nestCnt > -1 && headerLine.Any(c => c == '}'))
                    {
                        nestCnt -= headerLine.Count(c => c == '}');
                        if (nestCnt == 0)
                            nestCnt = -1;
                        else
                            builder.AppendLine(headerLine);
                    }
                    else if (headerLine.StartsWith(nameSpace))
                    {
                        nestCnt = 1;
                    }
                    else
                    {
                        builder.AppendLine(headerLine);
                    }
                }
            }
            else
            {
                if (nestCnt > -1 && line.Any(c => c == '{')) nestCnt += line.Count(c => c == '{');

                if (nestCnt > -1 && line.Any(c => c == '}'))
                {
                    nestCnt -= line.Count(c => c == '}');
                    if (nestCnt == 0)
                        nestCnt = -1;
                    else
                        builder.AppendLine(line);
                }
                else if (line.StartsWith(nameSpace))
                {
                    nestCnt = 1;
                }
                else
                {
                    builder.AppendLine(line);
                }
            }
        }

        var fileHeader = "#pragma once" + Environment.NewLine;

        CopyDirectory(linqDir, projectDir + "Release/Linq");
        if (!Directory.Exists(projectDir         + "Release/SingleHeader"))
            Directory.CreateDirectory(projectDir + "Release/SingleHeader");
        var rawInclude = includeList.Distinct().Aggregate((s, s1) => s + Environment.NewLine + s1) +
                         Environment.NewLine;

        File.WriteAllText(projectDir + "Release/SingleHeader/Linq.hpp",
                          fileHeader + rawInclude + defineBuilder + nameSpace + builder + "}", Encoding.UTF8);
    }
    
    private static void Main(string[] args)
    {
        var projectPath = args.Length == 0 ? "../../../../" : args[0] + "/";
        projectPath = Path.GetFullPath(projectPath);
        MakeRelease(projectPath);
        ZipFile.CreateFromDirectory(projectPath + "Release", projectPath + "Release.zip");
    }
}
}