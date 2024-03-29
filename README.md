## There's an updated version of this with additional visualizers for later versions of Visual Studio [here](https://github.com/Dave-Lowndes/DavesVisualStudioVisualizers/tree/master/CppCustomVisualizer)

# DavesEEAddin
An extended version of the Microsoft Visual Studio debugger sample expression evaluator EEAddin. It currently supports the following Windows types:

•	FILETIME  
•	SYSTEMTIME  
•	PROPERTYKEY  


![Demonstration animation](/demo.gif)

## Background

Visual Studio has a useful sample debugging expression evaluator for the FILETIME and SYSTEMTIME structures http://msdn.microsoft.com/en-us/library/8fwk67y3(v=vs.90).aspx . Unfortunately, the original supplied sample would crash because the exported functions don’t have the correct calling convention (_stdcall) defined. This version corrects this and extends the display of the FILETIME type to show the value interpreted as both UTC and local time as I’ve often found it useful to see both at the same time. It also displays PROPERTYKEY values in their canonical and display name forms.

See the **"Daves Debug Visualizer for Visual Studio 2008-2013.docx"** file in the GitHub code site for more information on how to use the DLL & natvis file in your Visual Studio installation.
