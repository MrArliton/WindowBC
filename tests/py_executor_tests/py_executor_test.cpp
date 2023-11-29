
// Library
#include "py_executor_test.h"

int main()
{
   std::cout << "Testing pyExec function:\n";  
   pyexec::pyInit();
   std::optional<PyObject*> response = pyexec::pyExecute("test_files.test_script", "test_func1", {PyLong_FromLong(10)});
   if(response && PyLong_AsLong(*response) == 11)
   {
        std::cout << "-- First test passed! --\n";
   }else
   {
        std::cout << "-- First test failed! --\n";
   }

   response = pyexec::pyExecute("test_files.test_script", "test_func2", {PyLong_FromLong(10),
   PyLong_FromLong(5)});

   if(response && PyLong_AsLong(*response) == 15)
   {
        std::cout << "-- Second test passed! --\n";
   }else
   {
        std::cout << "-- Second test failed! --\n";
   }

   response = pyexec::pyExecute("test_files.test_script", "test_func3", {PyUnicode_FromString("test")}); 
   if(response && std::string(PyUnicode_AsUTF8(*response)) == "testGood")
   {
        std::cout << "-- Third test passed! --\n";
   }else
   {
        std::cout << "-- Third test failed! --\n";
   }
   
   pyexec::pyFinalize();
}