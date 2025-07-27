### 【1925】report all immediate parents of work part open in session 报告会话中当前工作部件的直接上级部件

#### 代码

```cpp
    /*HEAD REPORT_ALL_IMMEDIATE_PARENTS_OF_WORK_PART_OPEN_IN_SESSION CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_part.h>  
    #include <uf_assem.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    /* qq3123197280 */  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    #define WRITENZ(X) if (X != 0) (write_integer_to_listing_window(#X, X))  
    /* qq3123197280 */  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        int  
            inx = 0,  
            count = 0;  
        tag_t  
            work_part = NULL_TAG,  
            * parent_parts = NULL;  
        char  
            fspec[MAX_FSPEC_SIZE + 1] = { "" },  
            msg[300] = { "" };  
        work_part = UF_ASSEM_ask_work_part();  
        if( NULL_TAG == work_part ) return;  
        WRITE_D( work_part );  
        UF_PART_ask_part_name(work_part, fspec);  
        sprintf( msg, "Work Part: %s\n",  fspec);  
        WRITE( msg );  
        count = UF_ASSEM_where_is_part_used( work_part, &parent_parts );  
        for( inx = 0; inx < count; inx++ )  
        {              
            sprintf(msg, "Parent Part Tag: %d\n", parent_parts[inx] );  
            WRITE( msg );  
            UF_PART_ask_part_name( parent_parts[inx], fspec );  
            sprintf( msg, "Parent Part Name: %s\n",  fspec);  
            WRITE( msg );  
        }  
        if( count > 0 ) UF_free( parent_parts );     
    }  
    /* qq3123197280 */  
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

> 这段代码是NX的二次开发代码，主要用于报告当前工作部件的所有直接父部件。
>
> 主要功能包括：
>
> 1. 获取当前工作部件的tag。
> 2. 获取工作部件的名称。
> 3. 使用UF_ASSEM_where_is_part_used函数查询工作部件的所有直接父部件，并将它们的tag和名称打印出来。
> 4. 使用UF_free释放分配的父部件数组。
> 5. 使用宏定义了错误报告函数report_error，用于打印出函数调用失败时的错误信息。
> 6. 使用宏定义了WRITE、WRITE_D、WRITENZ，用于将信息写入list窗口。
> 7. ufusr是NX调用的主函数，首先调用UF_initialize初始化，然后调用do_it执行主体逻辑，最后调用UF_terminate结束。
> 8. ufusr_ask_unload函数返回UF_UNLOAD_IMMEDIATELY，表示不需要卸载。
>
> 总体来说，这段代码实现了查询工作部件的直接父部件，并将它们的信息打印出来的功能。
>
