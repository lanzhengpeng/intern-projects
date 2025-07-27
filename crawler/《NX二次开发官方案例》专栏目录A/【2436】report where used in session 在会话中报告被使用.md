### 【2436】report where used in session 在会话中报告被使用

#### 代码

```cpp
    /*HEAD REPORT_WHERE_USED_IN_SESSION CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是在V18版本中新增的函数。 */  
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
    static void do_it(void)  
    {  
        int  
            ii,  
            n_occs,  
            n_parts = UF_PART_ask_num_parts();  
        tag_t  
            *occs,  
            parent,  
            work = UF_ASSEM_ask_work_part();  
        char  
            part_name[MAX_FSPEC_SIZE+1];  
        UF_PART_ask_part_name(work, part_name);  
        printf("%s is used in:\n", part_name);  
        for (ii = 0; ii < n_parts; ii++)  
        {  
            if (((parent = UF_PART_ask_nth_part(ii)) != NULL_TAG) &&  
                ((n_occs = UF_ASSEM_ask_occs_of_part(parent, work, &occs)) > 0))  
            {  
                UF_PART_ask_part_name(parent, part_name);  
                printf("\t%s\n", part_name);  
                UF_free(occs);  
            }  
        }  
    }  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段NX二次开发代码的主要功能是遍历当前会话中的所有组件，并找出哪些组件引用了指定的组件。
>
> 主要步骤包括：
>
> 1. 包含必要的NX Open头文件。
> 2. 定义错误报告函数report_error，用于输出错误信息。
> 3. 定义do_it函数，该函数遍历所有组件，并检查每个组件是否引用了指定的组件。
> 4. ufusr函数是用户自定义函数的入口，初始化NX Open环境后调用do_it函数，最后终止NX Open环境。
> 5. ufusr_ask_unload函数用于卸载用户自定义函数，返回UF_UNLOAD_IMMEDIATELY表示立即卸载。
>
> 通过遍历所有组件，检查引用关系，该代码实现了在当前会话中查找指定组件的用途。
>
