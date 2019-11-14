using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using BL.Helpers;

namespace Scripts
{
    class Program
    {
        static void Main(string[] args)
        {
            var path = PathHelper.GetResourcesPath();

            var files = Directory.GetFiles(path);

            foreach (var file in files)
            {
                var bitmap = new Bitmap(file);

                for (var x = 0; x < bitmap.Width; ++x)
                    for (var y = 0; y < bitmap.Height; ++y)
                    {
                        var pixel = bitmap.GetPixel(x, y);

                        if (pixel.R == 0 && pixel.G == 0 && pixel.B == 0)
                        {
                            var newAlpha = Color.FromArgb(0, 0, 0, 0);

                            bitmap.SetPixel(x, y, newAlpha);
                        }
                    }

                var fileName = Path.GetFileName(file);
                var fileNameWithoutExtension = Path.GetFileNameWithoutExtension(file);
                var extension = fileName.Replace(fileNameWithoutExtension, string.Empty);

                var newPath = file.Replace(fileName, string.Empty) + $"{fileNameWithoutExtension}.png";
                bitmap.Save(newPath);
            }
        }
    }
}
