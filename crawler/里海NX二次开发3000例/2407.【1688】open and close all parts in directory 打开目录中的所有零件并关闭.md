### 【1688】open and close all parts in directory 打开目录中的所有零件并关闭

#### 代码

```cpp
    /*HEAD OPEN_AND_CLOSE_ALL_PARTS_IN_DIRECTORY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_cfi.h>  
    #include <uf_part.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本中新增的函数。在翻译中，我们只需要回答翻译，不需要添加其他无关内容。因此，翻译为：

UF_print_syslog是V18版本中新增的函数。 */  
            UF_print_syslog(msg, FALSE);  
            UF_print_syslog(err, FALSE);  
            UF_print_syslog("\n", FALSE);  
            UF_print_syslog(call, FALSE);  
            UF_print_syslog(";\n", FALSE);  
            if (!UF_UI_open_listing_window())  
            {  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window(err);  
                UF_UI_write_listing_window("\n");  
                UF_UI_write_listing_window(call);  
                UF_UI_write_listing_window(";\n");  
            }  
        }  
        return(irc);  
    }  
    static void check_load_status(UF_PART_load_status_p_t status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        for (ii=0; ii<status->n_parts; ii++)  
        {  
            UF_get_fail_message(status->statuses[ii], msg);  
            printf("    %s - %s\n", status->file_names[ii], msg);  
        }  
        UF_free(status->statuses);  
        UF_free_string_array(status->n_parts, status->file_names);  
    }  
    int main( void )  
    {  
        int  
            cnt = 0,  
            resp;  
        tag_t  
            part;  
        char  
            dirspec[MAX_FSPEC_SIZE+1] = { "" },  
            fname[MAX_FSPEC_SIZE+1],  
            part_name[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            status;  
        if (UF_CALL(UF_initialize())) return 1;  
        while (printf("Enter directory path:\n") && gets(dirspec) &&  
            strlen(dirspec))  
        {  
            uc4508(dirspec, (1<<13)|(1<<11), 0, "*.prt");  
            while ((resp = uc4518()) == 0)  
            {  
                UF_CALL(uc4600(fname));  
                UF_CALL(uc4575(dirspec, 2, fname, part_name));  
                printf("%d.  %s\n", ++cnt, part_name);  
                UF_CALL(UF_PART_open(part_name, &part, &status));  
                if (status.n_parts > 0) check_load_status(&status);  
                if (!status.failed)  
                {  
                    printf("\topened OK.\n");  
                    UF_PART_close_all();  
                }  
            }  
            UF_CALL(uc4548());  
            printf("\nProcessed %d parts.\n", cnt);  
        }  
        UF_CALL(UF_terminate());  
        return 0;  
    }

```

#### 代码解析

> 这段代码是一个NX Open C++应用程序，其主要功能是打开指定目录中的所有prt文件，并在打开后关闭它们。
>
> 主要步骤如下：
>
> 1. 初始化NX Open。
> 2. 循环读取用户输入的目录路径，并使用NX Open的文件函数来获取该目录下所有prt文件的名称。
> 3. 遍历这些文件名，逐个打开prt文件，并检查打开状态。如果打开成功，则打印提示，并关闭所有已打开的prt文件。如果打开失败，则打印错误信息。
> 4. 在每次处理完一个目录后，打印已处理的prt文件数量。
> 5. 在程序结束前，关闭NX Open。
>
> 该程序使用了NX Open的文件函数和部件函数，通过命令行参数获取目录路径，打开所有prt文件并关闭它们。代码还包含错误处理和提示输出，以提供用户交互。
>
