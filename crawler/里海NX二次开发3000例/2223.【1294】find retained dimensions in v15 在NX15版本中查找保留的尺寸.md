### 【1294】find retained dimensions in v15 在NX15版本中查找保留的尺寸

#### 代码

```cpp
    /*HEAD FIND_RETAINED_DIMENSIONS_IN_V15 CCC UFUN */  
    /*  Here is the GRIP which returns the retained status of the dimension  
            NUMBER/n  
            ENTITY/ent  
            UFARGS/ent, n  
            n = &RETFLG(ent)  
            HALT  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #define ECHO(X)    (printf("\t%s = %d\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本中的新功能，请不要添加无关的评论。 */  
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
    static tag_t ask_next_dim(tag_t part, tag_t dim)  
    {  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_dimension_type, &dim))  
            && (dim != NULL_TAG))  
        {  
            if (!UF_ASSEM_is_occurrence(dim)) return dim;  
        }  
        return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        int  
            cnt = 0;  
        tag_t  
            ret_dim = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        double  
            n;  
        UF_args_t  
            arg[2];  
        char  
            messg[133];  
        arg[0].type = UF_TYPE_TAG_T;  
        arg[0].length = 0;  
        arg[0].address = &ret_dim;  
        arg[1].type = UF_TYPE_DOUBLE;  
        arg[1].length = 0;  
        arg[1].address = &n;  
        while ((ret_dim = ask_next_dim(part, ret_dim)) != NULL_TAG)  
        {  
    #ifdef _WIN32  
            UF_CALL(UF_call_grip("N:\\grip\\ask_dim_retained_status.grx", 2, arg));  
    #else  
            UF_CALL(UF_call_grip("grip/ask_dim_retained_status.grx", 2, arg));  
    #endif  
            if ((int)n == 1)  
            {  
                cnt++;  
                ECHO(ret_dim);  
                UF_CALL(UF_DISP_set_highlight(ret_dim, TRUE));  
            }  
        }  
        sprintf(messg, "Found %d retained dimensions", cnt);  
        uc1601(messg, TRUE);  
        if (cnt > 0) UF_CALL(UF_PART_cleanup(UF_PART_cleanup_highlight));  
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

> 这段代码是用于在NX中查找并高亮显示保留的尺寸的二次开发代码。下面是主要功能的介绍：
>
> 1. 使用UF_OBJ_cycle_objs_in_part函数遍历零件中的所有尺寸对象，并使用UF_ASSEM_is_occurrence函数判断尺寸是否为保留尺寸。
> 2. 如果是保留尺寸，调用一个名为ask_dim_retained_status的GRIP脚本，以获取该尺寸的保留状态。
> 3. 如果GRIP脚本返回保留状态为1，则使用UF_DISP_set_highlight函数将该尺寸高亮显示，并统计保留尺寸的数量。
> 4. 最后，使用uc1601函数显示找到的保留尺寸数量，并使用UF_PART_cleanup函数清理高亮显示。
> 5. ufusr函数初始化并执行上述功能，ufusr_ask_unload函数用于卸载二次开发程序。
> 6. 报错处理函数report_error用于打印错误信息。
> 7. ask_next_dim函数用于遍历零件中的尺寸对象。
> 8. do_it函数是主逻辑函数。
> 9. 使用UF_CALL宏封装UF函数调用，便于错误处理。
> 10. 在Windows和Linux平台下调用GRIP脚本的路径不同。
>
> 综上所述，该代码通过遍历尺寸对象，利用GRIP获取保留状态，实现了查找并高亮显示保留尺寸的功能。
>
