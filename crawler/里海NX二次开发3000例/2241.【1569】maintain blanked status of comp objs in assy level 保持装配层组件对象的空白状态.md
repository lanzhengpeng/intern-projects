### 【1569】maintain blanked status of comp objs in assy level 保持装配层组件对象的空白状态

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_part.h>  
    #include <uf_assem.h>  
    #include <uf_object_types.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
        }  
        return(irc);  
    }  
    static void set_object_blank(tag_t object)  
    {  
        int  
            object_blank,  
            prototype_blank;  
        tag_t  
            prototype;  
        UF_OBJ_disp_props_t  
            disp_props;  
        UF_CALL(UF_OBJ_ask_display_properties(object, &disp_props));  
        object_blank = disp_props.blank_status;  
        prototype = UF_ASSEM_ask_prototype_of_occ(object);  
        if( UF_OBJ_is_displayable( prototype ) )  
        {  
            UF_CALL(UF_OBJ_ask_display_properties(prototype, &disp_props));  
            prototype_blank = disp_props.blank_status;  
            if (object_blank != prototype_blank)  
            {  
                UF_CALL(UF_OBJ_set_blank_status(object, UF_OBJ_BLANKED));  
                UF_DISP_refresh();  
            }  
            else if (object_blank == prototype_blank)  
            {  
                UF_CALL(UF_OBJ_set_blank_status(prototype, UF_OBJ_NOT_BLANKED));  
            }  
        }  
    }  
    static tag_t ask_next_object(tag_t part, tag_t obj)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        int  
            subtype,  
            type;  
        while ((obj = UF_OBJ_cycle_all(part, obj)) != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(obj, &type, &subtype));  
            UF_CALL(UF_UI_open_listing_window());  
            sprintf(msg, "object_tag: %d, type: %d, subtype: %d\n", obj, type,  
                        subtype);  
            if (UF_ASSEM_is_occurrence(obj))  
            {  
                set_object_blank(obj);  
            }  
            else  
            {  
                sprintf(msg, "object is not an object occurence\n");  
            }  
            UF_CALL(UF_UI_write_listing_window(msg));  
        }  
        return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            obj_tag = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        ask_next_object(part, obj_tag);  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        UF_set_retiring_flag( 2 ); /* notify about retiring functions 里海译:通知关于废弃功能 */  
        do_it();  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这是NX Open API二次开发代码，其主要功能是遍历当前显示部件中的所有对象，并设置其显示属性。
>
> 代码的主要逻辑如下：
>
> 1. 包含必要的头文件，定义了一些宏和函数。
> 2. ECHO函数用于在日志窗口打印信息。
> 3. UF_CALL宏用于调用NX Open API函数，并报告错误。
> 4. report_error函数用于在发生错误时报告错误信息。
> 5. set_object_blank函数用于设置对象的显示属性，如果对象是原型，则根据原型显示属性来设置对象。
> 6. ask_next_object函数用于遍历部件中的所有对象，对每个对象调用set_object_blank函数设置显示属性。
> 7. do_it函数用于初始化并调用ask_next_object函数遍历对象。
> 8. ufusr函数是NX Open API二次开发的入口函数，调用do_it函数实现功能。
> 9. ufusr_ask_unload函数用于在卸载NX时立即卸载二次开发库。
>
> 总体来说，这段代码实现了在NX中遍历当前显示部件的所有对象，并根据其是否为原型来设置其显示属性的功能。
>
