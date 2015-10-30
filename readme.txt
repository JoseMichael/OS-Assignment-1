README

We have provided a rpc compiler that that reads specification files (.xml) and generates 3 files, the client stub, the server stub and header file.

client stub --> client_stub.c
server stub --> server_stub.c
header file --> rpcheader.c

The IDL, '.xml' format example is attached at the end.
Remember --> Only 1 and 2 dimensional arrays are accepted, 3 or more dimensional arrays will not work.
Please mention the size of the array before the array, in the parameter declaration both for 1 dimensional and 2 dimensional array.
 	eg: multiply(int a, int b, int* c, int d, int e, int* f)  //c and f are  2 dimensional arrays


MAKE SURE YOU HAVE WRITTEN YOUR server and client file (server.c and client.c resprectively)

-->SERVER FILE:
	Define and declare all the functions in this file, and make they match the specification file.

-->CLIENT FILE:
	Write all your code inside the function "void input(){}". This is the entry point to the client code.
	
RUNNNIG THE COMPILER:

/> gcc rpccompiler.c ezxml.c -o compiler
/> ./compiler

Enter the name of the specification file, on prompt.


RUN THE DIRECTORY SERVICE:

/> gcc directory.c -pthread -o directory
/>./directory

COMPILING AND RUNNING THE SERVER:

/> gcc server.c server_stub.c -pthread -o server
/>./server

COMPILING AND RUNNING THE CLIENT:

/> gcc client.c client_stub.c -o client
/> ./client

The specification file format: (Example)
<program>
	<id>1</id>
	<name>serverclient</name>
	<version>2</version>
	<procedure>
		<name>max</name>
		<id>1</id>
		<return>int</return>
		<param>
			<name>size</name>
			<index>1</index>
			<type>int</type>
		</param>
		<param>
			<name>intarray</name>
			<index>2</index>
			<type>int*</type>
		</param>
	</procedure>
	<procedure>
		<name>sort</name>
		<id>5</id>
		<return>int*</return>
		<param>
			<name>size</name>
			<index>1</index>
			<type>int</type>
		</param>
		<param>
			<name>array</name>
			<index>2</index>
			<type>int*</type>
		</param>
	</procedure>
</program>
