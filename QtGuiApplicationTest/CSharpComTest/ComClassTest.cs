using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace CSharpComTest
{
    [ComVisible(true)]
    [Guid("B6AD3E87-5415-495E-BCB7-E5C31FDD8F1E")]
    public interface IComInterface
    {
        [DispId(1)]
        int Add(int a, int b);
        [DispId(2)]
        int Minus(int a, int b);
        [DispId(3)]
        int Multiple(int a, int b);
        [DispId(4)]
        float Devide(float a, float b);
    }

    [ComVisible(true)]
    [Guid("72E5AA0A-1CF3-46BA-8D56-50C56AE0F338")]
    [ProgId("CSharpComTest.ComClassTest")]
    public class ComClassTest : IComInterface
    {
        public int Add(int a, int b)
        {
            return a + b;
        }

        public int Minus(int a, int b)
        {
            return a - b;
        }

        public int Multiple(int a, int b)
        {
            return a * b;
        }

        public float Devide(float a, float b)
        {
            return a / b;
        }
    }
}
