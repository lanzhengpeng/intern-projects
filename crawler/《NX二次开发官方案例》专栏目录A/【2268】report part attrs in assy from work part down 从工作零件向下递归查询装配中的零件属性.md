### 【2268】report part attrs in assy from work part down 从工作零件向下递归查询装配中的零件属性

#### 代码

```cpp
    /*HEAD REPORT_PART_ATTRS_IN_ASSY_FROM_WORK_PART_DOWN CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_part.h>  
    #include <uf_attr.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本新增的功能。

综上所述，UF_print_syslog是V18版本新增的功能。 */  
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
    extern DllExport void find_part_attributes( tag_t comp )  
    {  
        int  
            inx = 0,  
            num = 0;  
        tag_t  
            part = NULL_TAG;  
        char  
            str[UF_ATTR_MAX_STRING_LEN+1],  
            part_name[MAX_FSPEC_SIZE + 1] = { "" };  
        UF_ATTR_part_attr_t  
            *attrs;  
        UF_ATTR_value_t  
            value;  
        value.type = UF_ATTR_string;  
        value.value.string = str;  
        if( UF_ASSEM_is_occurrence( comp ) == TRUE )  
        {  
            part = UF_ASSEM_ask_prototype_of_occ( comp );     
        }  
        else  
        {  
            part = comp;     
        }  
        UF_CALL( UF_PART_ask_part_name( part, part_name ));  
        WRITE( "Processing: " );  
        WRITE( part_name );  
        WRITE( "...\n" );  
        UF_CALL(UF_ATTR_ask_part_attrs( part, &num, &attrs ));  
        if( 0 == num ) return;    /* No Part Attributes were found 里海译:无部分属性找到 */  
        for (inx = 0; inx < num; inx++)  
        {  
            WRITE( attrs[inx].title );  
            WRITE( "  " );  
            WRITE( attrs[inx].string_value );  
            WRITE( "\n" );  
        }  
        if (num > 0) UF_free( attrs);  
    }  
    /* qq3123197280 */  
    extern DllExport void get_part_attrs_in_assy( tag_t component )  
    {  
        int  
            ii,  
            n_children;  
        tag_t  
            *child_components;  
        n_children = UF_ASSEM_ask_part_occ_children( component, &child_components );  
        if (n_children > 0)  
        {  
            for (ii = 0; ii < n_children; ii++)  
            {  
                find_part_attributes( child_components[ii] );  
               get_part_attrs_in_assy( child_components[ii] );  
            }  
            if ( n_children > 0 ) UF_free( child_components );  
        }  
    }  
    /* qq3123197280 */  
    static void do_it(void)  
    {  
        tag_t  
            original_work_part = NULL_TAG,  
            root_part_occ = NULL_TAG;  
        char  
            part_name[MAX_FSPEC_SIZE + 1] = { "" };  
        original_work_part = UF_ASSEM_ask_work_part();  
        UF_CALL( UF_PART_ask_part_name( original_work_part, part_name ));  
        WRITE( "Processing: " );  
        WRITE( part_name );  
        WRITE( "...\n" );  
        if( NULL_TAG != original_work_part )  
        {  
            find_part_attributes( original_work_part );  
        }  
        /*       
                walk the assembly tree, for each component found,  
                perform the find_part_attributes() function  
        */  
        root_part_occ = UF_ASSEM_ask_root_part_occ( original_work_part );  
        if (root_part_occ != NULL_TAG) get_part_attrs_in_assy( root_part_occ );      
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

> 这段NX二次开发代码的功能是递归遍历装配体中的所有零件，并打印出每个零件的名称和属性。
>
> 主要步骤如下：
>
> 1. 初始化：调用UF_initialize()来初始化NX环境。
> 2. 获取当前工作零件：通过UF_ASSEM_ask_work_part()获取当前的工作零件。
> 3. 打印工作零件的名称和属性：调用find_part_attributes()函数来打印工作零件的名称和属性。
> 4. 递归遍历装配体中的零件：通过get_part_attrs_in_assy()函数递归遍历装配体中的所有零件，打印每个零件的名称和属性。
> 5. 终止环境：调用UF_terminate()来终止NX环境。
>
> 其中，find_part_attributes()函数用于获取和打印零件的名称和属性，get_part_attrs_in_assy()函数则递归调用自身来遍历装配体中的所有零件。
>
> 此外，代码中还包含错误报告和日志打印的功能，以及用于释放内存的UF_free()调用。
>
> 综上所述，这段代码实现了遍历装配体中所有零件，并打印出每个零件的名称和属性的功能。
>
