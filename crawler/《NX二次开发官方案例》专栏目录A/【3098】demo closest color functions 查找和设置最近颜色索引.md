### 【3098】demo closest color functions 查找和设置最近颜色索引

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_disp.h>  
    #include <uf_obj.h>  
    #include <uf_part.h>  
    #include <stdarg.h>  
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
    static tag_t ask_next_body(tag_t part, tag_t body)  
    {  
        int  
            subtype,  
            type;  
        while (! UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &body))  
            && (body != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
            if (subtype == UF_solid_body_subtype) return body;  
        }  
        return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        int  
            blue_index;  
        tag_t  
            part = UF_PART_ask_display_part(),  
            body = ask_next_body(part, NULL_TAG);  
        double  
            blue_rgb[3] = { 0.0, 0.0, 1.0 };  
        UF_CALL(UF_DISP_ask_closest_color(UF_DISP_rgb_model, blue_rgb,  
            UF_DISP_CCM_EUCLIDEAN_DISTANCE, &blue_index));  
        ECHO("UF_DISP_ask_closest_color:  blue_index = %d\n", blue_index);  
        UF_CALL(UF_DISP_ask_closest_color_in_displayed_part(UF_DISP_BLUE_NAME,  
            &blue_index));  
        ECHO("UF_DISP_ask_closest_color_in_displayed_part:  blue_index = %d\n",  
            blue_index);  
        UF_CALL(UF_OBJ_set_color(body, blue_index));  
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

> 这段NX Open C++代码用于更改NX显示部件中实体的颜色。
>
> 主要功能包括：
>
> 1. 定义了一个ECHO宏，用于输出信息到日志窗口和控制台。
> 2. 定义了一个UF_CALL宏，用于调用NX API函数，并在出错时输出错误信息。
> 3. ask_next_body函数用于遍历显示部件中的实体，并返回下一个实体的标签。
> 4. do_it函数是主函数，获取当前显示部件，遍历实体，并使用ask_next_body获取下一个实体。然后，该函数使用UF_DISP_ask_closest_color函数查找与蓝色最接近的颜色索引，并输出该索引。接着，它使用UF_DISP_ask_closest_color_in_displayed_part函数在显示部件中查找最接近的颜色索引，并输出该索引。最后，该函数使用UF_OBJ_set_color函数将实体的颜色设置为找到的颜色索引。
> 5. ufusr函数是NX Open提供的回调函数，在启动NX时调用。它初始化NX Open，调用do_it函数执行主要功能，然后终止NX Open。
> 6. ufusr_ask_unload函数用于卸载用户函数，返回UF_UNLOAD_IMMEDIATELY表示立即卸载。
>
> 整体来看，这段代码的主要目的是实现更改NX显示部件中实体颜色的功能，通过遍历实体，查找并设置颜色。
>
