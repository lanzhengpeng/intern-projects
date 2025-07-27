### 【1581】make window for printf on wnt 在WNT上为printf创建窗口

#### 代码

```cpp
    /*HEAD MAKE_WINDOW_FOR_PRINTF_ON_WNT.CPP CCC UFUN */  
    /*  
        The output from the C command "printf" is no longer  
        displayed in any window onthe WNT platform starting in UG  
        V16.0.  
        V16.0 UG is a Windows Application and no longer has a  
        console tied to the process so there is no place to view the  
        "printf" output.  
        To work around this problem, build a shared library using  
        this code and place the resulting .dll in the  
        "startup" or "udo" subdirectory of a directory listed in  
        %UGII_CUSTOM_DIRECTORY_FILE%.  This program will create a  
        window each time UG starts up which can then be used to view  
        stdout for the remainder of the session.  
        For more information on the "startup" or "udo"  
        subdirectories or the UGII_CUSTOM_DIRECTORY_FILE environment  
        variable, consult the UG/Open Menuscript documentation.  
    */  
    #include <windows.h>  
    #include <fstream.h>  
    #include <stdio.h>  
    #include <uf_exit.h>  
    static filebuf coutbuf;  
    void ufsta(char *param, int *retcode, int paramLen)  
    {  
        ostream coutstr(&coutbuf);  
        // allocate console window conout$  
        AllocConsole();  
        // reopen stdout on conout$ for fprintf and printf  
        freopen("conout$", "w", stdout);  
        // Avoid buffer overflows  
        setvbuf(stdout,NULL,_IONBF,2);   
        // open filebuf on conout$, create ostream, and assign to cout  
        coutbuf.open("conout$", ios::out);  
        cout = coutstr;  
        printf( "printf's ");  
        cout << "and cout's will go here now" << endl;  
    }

```

#### 代码解析

> 这段代码是一个NX Open二次开发代码，主要功能是在Windows平台下为NX创建一个窗口，以便显示printf的输出。具体来说，它通过以下关键步骤实现这一功能：
>
> 1. 包含必要的头文件，包括windows.h、fstream.h、stdio.h和uf_exit.h。
> 2. 定义一个全局的文件缓冲区coutbuf，用于重定向cout的输出。
> 3. 定义ufsta函数，该函数会在NX启动时被调用。在ufsta函数中，首先创建一个基于coutbuf的输出流coutstr。
> 4. 调用AllocConsole()创建一个控制台窗口，这样printf的输出就有地方显示了。
> 5. 通过freopen(“conout$”, “w”, stdout)将标准输出stdout重定向到控制台窗口。
> 6. 设置stdout的缓冲区为无缓冲，避免缓冲区溢出。
> 7. 打开coutbuf，创建一个输出流，并将其赋值给cout。这样，cout的输出也会重定向到控制台窗口。
> 8. 在控制台窗口打印"printf’s "。
> 9. 通过cout输出"and cout’s will go here now"，这样cout的输出也会显示在控制台窗口。
>
> 综上所述，这段代码通过在NX启动时调用，创建一个控制台窗口，并将标准输出和cout重定向到该窗口，从而实现了在NX中显示printf和cout输出的功能。
>
