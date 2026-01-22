using System;
using System.Runtime.InteropServices;

namespace ComMathClient
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("COM Math Client - C# Example");
            Console.WriteLine("============================\n");

            try
            {
                // Create instance of the COM Math class
                // CLSID: ECC3A324-8EB0-4B88-8D52-3A9281C0D0AD
                Type mathType = Type.GetTypeFromProgID("ComMath.Math");
                if (mathType == null)
                {
                    Console.WriteLine("ERROR: COM class not registered!");
                    Console.WriteLine("Please register the COM DLL first using:");
                    Console.WriteLine("  regsvr32 ComMath.dll");
                    return;
                }

                dynamic math = Activator.CreateInstance(mathType);

                // Test the Value property
                Console.WriteLine("Testing Value property:");
                math.Value = 42.5;
                Console.WriteLine($"  Set Value = 42.5");
                Console.WriteLine($"  Get Value = {math.Value}\n");

                // Test Add method
                double a = 10.5;
                double b = 5.25;
                double result = math.Add(a, b);
                Console.WriteLine($"Add({a}, {b}) = {result}");

                // Test Subtract method
                result = math.Subtract(a, b);
                Console.WriteLine($"Subtract({a}, {b}) = {result}");

                // Test Multiply method
                result = math.Multiply(a, b);
                Console.WriteLine($"Multiply({a}, {b}) = {result}");

                // Test Divide method
                result = math.Divide(a, b);
                Console.WriteLine($"Divide({a}, {b}) = {result}");

                // Test division by zero
                Console.WriteLine("\nTesting error handling:");
                try
                {
                    result = math.Divide(a, 0);
                    Console.WriteLine($"Divide({a}, 0) = {result}");
                }
                catch (COMException ex)
                {
                    Console.WriteLine($"  Division by zero caught: HRESULT = 0x{ex.HResult:X8}");
                }

                // Release COM object
                Marshal.ReleaseComObject(math);
                Console.WriteLine("\nCOM object released successfully.");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"ERROR: {ex.Message}");
                Console.WriteLine($"Type: {ex.GetType().Name}");
                if (ex.InnerException != null)
                {
                    Console.WriteLine($"Inner: {ex.InnerException.Message}");
                }
            }

            Console.WriteLine("\nPress any key to exit...");
            Console.ReadKey();
        }
    }
}
