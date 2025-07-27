### 【1297】find sketch for each UF sketch tol csys type object 找到每个UF草图公差坐标系类型对象的草图

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_layer.h>  
    #include <uf_disp.h>  
    #include <uf_assem.h>  
    #include <uf_part.h>  
    #include <uf_text.h>  
    #include <uf_ui.h>  
    #include <uf_sket.h>  
    #include <stdarg.h>  
    #pragma warning(disable:4996)  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN + 1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error(char *file, int line, char *call, int irc)  
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
    static tag_t ask_next_of_type(tag_t part, int type, tag_t object)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, type, &object));  
        return (object);  
    }  
    static tag_t ask_sketch_of_csys(tag_t theCsys)  
    {  
        // NOTE:  If you use this function in a different program,  
        // you should type-check your input to be sure that you are  
        // passing in an object of type UF_sketch_tol_csys_type  
        tag_t  
            skt = NULL_TAG,  
            proto = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        if (UF_ASSEM_is_occurrence(theCsys) == TRUE)  
        {  
            proto = UF_ASSEM_ask_prototype_of_occ(theCsys);  
            UF_CALL(UF_OBJ_ask_owning_part(proto, &part));  
        }  
        else  
        {  
            proto = theCsys;  
        }  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_sketch_type, &skt));  
        while (skt != NULL_TAG)  
        {  
            UF_SKET_info_t  
                sktInfo;  
            UF_CALL(UF_SKET_ask_sketch_info(skt, &sktInfo));  
            if (sktInfo.csys_tag == proto) return(skt);  
            UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_sketch_type, &skt));  
        }  
        return(NULL_TAG);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            part = UF_PART_ask_display_part(),  
            thing = NULL_TAG,  
            theSketch = NULL_TAG;  
        UF_OBJ_disp_props_t  
            disp_props;  
        do  
        {  
            theSketch = NULL_TAG;  
            thing = ask_next_of_type(part, UF_sketch_tol_csys_type, thing);  
            if (NULL_TAG == thing) break;  
            UF_CALL(UF_OBJ_ask_display_properties(thing, &disp_props));  
            if (disp_props.layer > 0 && disp_props.layer < 257)    
            {  
                theSketch = ask_sketch_of_csys(thing);  
                ECHO("Csys: %d  Sketch: %d\n", thing, theSketch);  
            }  
        } while (thing != NULL_TAG);  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        UF_CALL(UF_TEXT_set_text_mode(UF_TEXT_ALL_UTF8));  
        do_it();  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是一段NX Open C++ API编写的NX二次开发代码，主要功能是遍历当前显示部件中的草图坐标系，并找出与之关联的草图。
>
> 关键要点包括：
>
> 1. 错误报告：定义了ECHO函数用于打印信息到列表窗口和系统日志，以及report_error函数用于在函数调用出错时报告错误。
> 2. 对象遍历：定义了ask_next_of_type函数，用于遍历部件中的指定类型对象。
> 3. 关联草图查找：定义了ask_sketch_of_csys函数，用于根据坐标系找到与之关联的草图。
> 4. 遍历并打印：do_it函数遍历当前显示部件中的坐标系，并找到关联的草图，然后打印坐标系和草图标签。
> 5. API初始化：ufusr函数初始化API，设置文本模式，调用do_it执行功能，然后终止API。
> 6. 卸载提示：ufusr_ask_unload函数用于提示立即卸载用户函数。
>
> 总体来说，这段代码实现了遍历坐标系并找到关联草图的功能，通过错误处理、对象遍历等手段实现NX二次开发。
>
