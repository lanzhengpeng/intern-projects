### 【1209】export cgm using default scale 导出CGM时使用默认比例

#### 代码

```cpp
    /*HEAD EXPORT_CGM_USING_DEFAULT_SCALE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_part.h>  
    #include <uf_std.h>  
    #include <uf_cfi.h>  
    #include <uf_view.h>  
    #include <uf_draw.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文中的注释，UF_print_syslog 是 V18 版本中新增的函数。 */  
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
    static void reset_cgm_parameters_to_default_values(tag_t part)  
    {  
        int  
            type,  
            subtype;  
        tag_t  
            object = NULL_TAG;  
        while (! UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_parameter_type, &object))  
            &&  object != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype( object, &type, &subtype));  
            if (subtype == UF_parm_diagram_subtype)  
            {  
                UF_CALL(UF_OBJ_delete_object(object));  
                break;  
            }  
        }  
    }  
    static tag_t ask_next_drawing(tag_t part, tag_t drawing)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_drawing_type, &drawing));  
        return (drawing);  
    }  
    static void do_it(void)  
    {  
        int  
            pens[15] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};  
        tag_t  
            drawing = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        char  
            draw_name[MAX_ENTITY_NAME_SIZE+1],  
            export_fspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            part_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_fspec[UF_CFI_MAX_PATH_NAME_SIZE];  
        reset_cgm_parameters_to_default_values(part);  
        UF_PART_ask_part_name(part, part_fspec);  
        UF_CALL(uc4574(part_fspec, 2, part_name));  
        while ((drawing = ask_next_drawing(part, drawing)) != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_name(drawing, draw_name));  
            sprintf(part_fspec, "%s-%s", part_name, draw_name);  
            part_fspec[26] = '\0';  /* In case it is too long 里海译:In case it is too long, 如果太长 */  
            UF_CALL(uc4575("/tmp", 94, part_fspec, export_fspec));  
            UF_CALL(uc6410(draw_name, export_fspec, 2, 1, pens));  
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

> 这是NX二次开发代码，主要功能是遍历当前活动部件中的所有图纸，并将每个图纸导出为CGM格式的文件。
>
> 主要逻辑包括：
>
> 1. 初始化NX环境。
> 2. 获取当前活动部件。
> 3. 重置当前活动部件的CGM参数为默认值，以确保导出的CGM文件使用默认参数。
> 4. 遍历当前活动部件中的所有图纸，并获取每个图纸的名称。
> 5. 构造每个CGM文件的名称，格式为"部件名称-图纸名称.cgm"。
> 6. 使用NX提供的函数，将每个图纸导出为CGM文件。
> 7. 完成导出后，终止NX环境。
>
> 代码通过循环遍历当前活动部件中的所有图纸，并逐个导出为CGM文件，实现了图纸批量导出的功能。
>
