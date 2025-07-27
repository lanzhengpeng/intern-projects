### 【2442】report whether visible parts list section exists 报告是否可见零件清单部分存在

#### 代码

```cpp
    /*HEAD REPORT_WHETHER_VISIBLE_PARTS_LIST_SECTION_EXISTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_obj.h>  
    #include <uf_part.h>  
    #include <uf_plist.h>  
    #include <uf_tabnot.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
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
    /* qq3123197280 */  
    static tag_t ask_next_parts_list(tag_t part, tag_t plist)  
    {  
        int  
            type = 0,  
            subtype = 0;  
        UF_OBJ_cycle_objs_in_part(part, UF_tabular_note_type, &plist);  
        if( NULL_TAG != plist )  
        {  
            UF_CALL( UF_OBJ_ask_type_and_subtype( plist, &type, &subtype ));  
            if( subtype == UF_parts_list_subtype ) return (plist);  
        }  
        return( NULL_TAG );  
    }  
    static void do_it(void)  
    {  
        tag_t  
            plist = NULL_TAG,  
            section = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        if( NULL_TAG == part ) return;  
        while ((plist = ask_next_parts_list( part, plist )) != NULL_TAG )  
        {  
            UF_TABNOT_ask_nth_section( plist, 0, &section );  
            if( section != NULL_TAG )  
            {  
                WRITE( "Visible parts list found\n\n" );  
                UF_CALL(UF_OBJ_set_name(section, "NX_BOM"));  
                return;  
            }  
        }  
        WRITE( "Visible parts list NOT found\n\n" );  
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

> 这段代码是一个NX Open二次开发示例，其功能是检查当前显示的零件中是否包含可见的零件清单部分，并执行以下操作：
>
> 1. 包含头文件，以支持通用函数、对象操作、零件操作、属性列表操作、表格注释操作和用户界面操作。
> 2. 定义UF_CALL宏，用于调用NX函数，并在出错时报告错误信息。
> 3. 定义ask_next_parts_list函数，用于遍历指定零件中的表格注释对象，并找到下一个零件清单属性列表对象。
> 4. 定义do_it函数，用于遍历当前显示零件中的所有零件清单属性列表，并检查第一个部分是否为可见的零件清单部分。若找到，则将其名称设置为NX_BOM，并返回。若未找到，则输出提示信息。
> 5. 定义ufusr函数，用于在NX启动时初始化并调用do_it函数，并在结束时终止NX。
> 6. 定义ufusr_ask_unload函数，用于在NX卸载时立即卸载此用户函数。
>
> 总体来说，该代码实现了对当前显示零件中可见零件清单部分的检查和重命名功能。
>
