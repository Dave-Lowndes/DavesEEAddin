# DavesEEAddin
A Visual Studio debugger expression evaluator based on the Microsoft EEAddin example. It currently supports: FILETIME, SYSTEMTIME, PROPERTYKEY

This is an expanded version of the Microsoft sample expression evaluator EEAddin. It currently supports the following Windows types:

•	FILETIME

•	SYSTEMTIME

•	PROPERTYKEY

## Background
Visual Studio has a useful sample debugging expression evaluator for the FILETIME and SYSTEMTIME structures http://msdn.microsoft.com/en-us/library/8fwk67y3(v=vs.90).aspx . Unfortunately, the original supplied sample would crash because the exported functions don’t have the correct calling convention (_stdcall) defined. This version corrects this and extends the display of the FILETIME type to show the value interpreted as both UTC and local time as I’ve often found it useful to see both at the same time. It also displays PROPERTYKEY values in their canonical and display name forms.

