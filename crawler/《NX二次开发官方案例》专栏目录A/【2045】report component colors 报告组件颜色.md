### 【2045】report component colors 报告组件颜色

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_assem.h>  
    #include <uf_cfi.h>  
    #include <uf_part.h>  
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
    static tag_t ask_next_component(tag_t part, tag_t comp)  
    {  
        int  
            subtype,  
            type;  
        while  (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_component_type, &comp))  
            && (comp != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(comp, &type, &subtype));  
            if (subtype == UF_part_occurrence_subtype) return comp;  
        }  
        return comp;  
    }  
    /*  Prior to NX 4.0, the returned full color name looked like this:  
        full_color_name = "2 (Green)"  
        So the method shown here cleans it up and returns just the name.  
    */  
    static void ask_color_name(int color_num, char *color_name)  
    {  
        double  
            clr_values[3];  
        char  
            *full_color_name;  
        UF_CALL(UF_DISP_ask_color(color_num, UF_DISP_rgb_model, &full_color_name,  
            clr_values));  
        if (strchr(full_color_name, '('))  
        {  
            strcpy(color_name, strchr(full_color_name, '(')+1);  
            strcpy(strchr(color_name, ')'), "");  
        }  
        else  
            strcpy(color_name, full_color_name);  
        UF_free(full_color_name);  
    }  
    static void ask_object_color_name(tag_t obj, char *color)  
    {  
        logical  
            is_disp;  
        UF_OBJ_disp_props_t  
            disp_props;  
        UF_CALL(UF_OBJ_is_displayable(obj, &is_disp));  
        if (is_disp)  
        {  
            UF_CALL(UF_OBJ_ask_display_properties(obj, &disp_props));  
            ask_color_name(disp_props.color, color);  
        }  
        else strcpy(color, "NOT DISPLAYABLE");  
    }  
    static void ask_component_partname(tag_t component, char *partname)  
    {  
        tag_t  
            instance;  
        double  
            csys_matrix[9],  
            origin[3],  
            transform[4][4];  
        char  
            instance_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            refset_name[MAX_ENTITY_NAME_SIZE+1];  
        if ((instance = UF_ASSEM_ask_inst_of_part_occ(component)) != NULL_TAG)  
        {  
            UF_CALL(UF_ASSEM_ask_part_name_of_child(instance, partname));  
        }  
        else  
        {  
            UF_CALL(UF_ASSEM_ask_component_data(component, partname, refset_name,  
                instance_name, origin, csys_matrix, transform));  
        }  
    }  
    static void ask_component_shortname(tag_t component, char *shortname)  
    {  
        logical  
            nxman;  
        char  
            partname[UF_CFI_MAX_PATH_NAME_SIZE];  
        ask_component_partname(component, partname);  
        UF_CALL(UF_is_ugmanager_active(&nxman));  
        if (nxman)  
            UF_CALL(UF_PART_name_for_display(partname, shortname));  
        else  
            UF_CALL(uc4574(partname, 2, shortname));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            component = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        char  
            color[MAX_ENTITY_NAME_SIZE+1],  
            comp_name[MAX_ENTITY_NAME_SIZE+1];  
        UF_CALL( UF_DISP_set_name_display_status ( UF_DISP_NAME_DISPLAY_ON ));  
        while ((component = ask_next_component(part, component)) != NULL_TAG)  
        {  
            ask_component_shortname(component, comp_name);  
            ask_object_color_name(component, color);  
            ECHO("%s color is %s\n", comp_name, color);  
        }  
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

> 这段NX Open C++代码用于遍历装配体中的所有零件，并输出每个零件的颜色和名称。以下是代码的主要功能和实现细节：
>
> 1. 错误报告函数：report_error函数用于报告函数调用失败，并输出错误代码和错误信息。
> 2. 遍历零件：ask_next_component函数用于遍历装配体中的所有零件，找到下一个零件并返回其tag。
> 3. 获取颜色名称：ask_color_name函数用于获取颜色编号对应的颜色名称。
> 4. 获取对象颜色名称：ask_object_color_name函数用于获取指定对象的颜色名称。
> 5. 获取零件名称：ask_component_partname函数用于获取零件的完整名称。
> 6. 获取零件简称：ask_component_shortname函数用于获取零件的简称。
> 7. 主函数：do_it函数遍历装配体中的所有零件，调用上述函数获取每个零件的颜色和名称，并输出。
> 8. ufusr入口函数：这是用户自定义函数的入口点，在NX启动时调用。
> 9. 卸载函数：ufusr_ask_unload函数用于处理自定义函数的卸载。
>
> 总的来说，这段代码通过遍历零件并获取其颜色和名称，实现了输出装配体中所有零件的颜色和名称的功能。
>
