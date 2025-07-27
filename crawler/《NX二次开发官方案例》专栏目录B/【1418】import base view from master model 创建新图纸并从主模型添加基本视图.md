### 【1418】import base view from master model 创建新图纸并从主模型添加基本视图

#### 代码

```cpp
    /*****************************************************************************  
    ** import_base_view_from_master_model.c  
    ** Creates a new Drawing Sheet and adds a base view from the master part.  
    ** The master is the first and only component of the drawing in native NX.  
    *****************************************************************************/  
    /* Include files 里海译:翻译：包含文件 */  
    #include <stdarg.h>  
    #include <stdio.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_assem.h>  
    #include <uf_draw.h>  
    #include <uf_drf.h>  
    #include <uf_obj.h>  
    #include <uf_part.h>  
    #include <uf_view.h>  
    #include <uf_ui.h>  
    #include <uf_ui_types.h>  
    //***  Write to Listing Window and Syslog  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_BUFSIZE];  
        va_list args;  
        va_start(args, format);  
        vsnprintf_s(msg, sizeof(msg), _TRUNCATE, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    //*** Error reporting  
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
    //***  ask for a view name of the master component part  
    static logical prompt_for_viewname(char *prompt, char *vname)  
    {  
        int  
            n,  
            resp;  
        resp = uc1600(prompt, vname, &n);  
        if (resp == 3 || resp == 5) return TRUE;  
        return FALSE;  
    }  
    //***  A program must do what a program has to do  
    static void do_it(void)  
    {  
        tag_t  
            view_to_add = NULL_TAG,  
            owning_part = NULL_TAG,  
            drawing = NULL_TAG,  
            drawing_part = UF_PART_ask_display_part(),  
            comp = NULL_TAG,  
            comp_part = NULL_TAG,  
            draw_view_tag = NULL_TAG;  
        char  
            dname[31] = { "TEST_DWG" },  
            vname[31] = { "TRIMETRIC" }, // or "TOP", "FLAT-PATTERN#1", etc.  
            cname[MAX_FSPEC_SIZE + 1] = { "" };  
        double  
            dwg_ref_pt[2] = { 150., 150. };  
        UF_DRAW_info_t  
            draw_info = { UF_DRAW_METRIC_SIZE, UF_DRAW_A2, 1.0, UF_PART_METRIC,    UF_DRAW_FIRST_ANGLE_PROJECTION };  
        UF_DRAW_view_info_t  
            view_info;  
        if (drawing_part == NULL_TAG) return;  
        UF_CALL(UF_OBJ_cycle_objs_in_part(drawing_part, UF_component_type, &comp));  
        if (comp == NULL_TAG)  
        {  
            ECHO("No component found. Exit.");  
            return;  
        }  
        comp_part = UF_ASSEM_ask_prototype_of_occ(comp);  
        UF_CALL(UF_PART_ask_part_name(comp_part, cname));  
        prompt_for_viewname("Enter view name to import:", vname);  
        UF_CALL(UF_OBJ_cycle_by_name_and_type(comp_part, vname, UF_view_type, false, &view_to_add));  
        if (view_to_add == NULL_TAG)  
        {  
            ECHO("No view <%s> found in part <%s>. Exit.", vname, cname);  
            return;  
        }  
        UF_CALL(UF_OBJ_cycle_by_name_and_type(drawing_part, dname, UF_drawing_type, false, &drawing));  
        if (drawing == NULL_TAG)   
            UF_CALL(UF_DRAW_create_drawing(dname, &draw_info, &drawing));  
        UF_DRAW_initialize_view_info(&view_info);  
        view_info.view_status = UF_DRAW_ACTIVE_VIEW;  
        view_info.anchor_point = NULL_TAG;  
        view_info.view_scale = 1.0;  
        view_info.use_ref_pt = FALSE;  
        view_info.clean_model_view = FALSE;  
        view_info.inherit_boundary = FALSE;  
        view_info.transfer_annotation = FALSE;  
        strcpy(view_info.model_name, cname);  
        view_info.arrangement_name[0] = '\0';  
        UF_CALL(UF_DRAW_import_view(drawing, view_to_add, dwg_ref_pt, &view_info, &draw_view_tag));  
        UF_CALL(UF_DRAW_update_one_view(drawing, draw_view_tag));  
    }  
    /*****************************************************************************  
    **  Activation Methods  
    *****************************************************************************/  
    /*  Explicit Activation  
    **      This entry point is used to activate the application explicitly, as in  
    **      "File->Execute UG/Open->User Function..." */  
    extern DllExport void ufusr( char *parm, int *returnCode, int rlen )  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            do_it();  
            UF_CALL(UF_terminate());  
        }  
    }  
    /*****************************************************************************  
    **  Utilities  
    *****************************************************************************/  
    extern int ufusr_ask_unload( void )  
    {  
        return( UF_UNLOAD_IMMEDIATELY );  
    }

```

#### 代码解析

> 这段代码是一个用于在NX中创建新图纸并从主零件添加基础视图的二次开发程序。以下是代码的主要功能介绍：
>
> 1. 包含必要的NX开发头文件。
> 2. 定义了ECHO函数，用于在列表窗口和系统日志中输出信息。
> 3. 定义了UF_CALL宏，用于调用NX API并报告错误。
> 4. 定义了prompt_for_viewname函数，用于提示用户输入要导入的视图名称。
> 5. do_it函数是程序的主体，包括以下步骤：获取图纸中的第一个组件。获取组件对应的主模型。提示用户输入要导入的视图名称。根据视图名称获取视图对象。根据图纸名称获取或创建图纸对象。初始化视图信息结构。将视图导入到图纸中。更新图纸中的视图。
> 6. 获取图纸中的第一个组件。
> 7. 获取组件对应的主模型。
> 8. 提示用户输入要导入的视图名称。
> 9. 根据视图名称获取视图对象。
> 10. 根据图纸名称获取或创建图纸对象。
> 11. 初始化视图信息结构。
> 12. 将视图导入到图纸中。
> 13. 更新图纸中的视图。
> 14. ufusr函数是程序的入口点，用于初始化NX环境，调用do_it函数，然后终止NX环境。
> 15. ufusr_ask_unload函数用于卸载插件。
>
> 整体来看，该代码实现了在NX中创建新图纸并从主零件导入指定视图的功能，方便用户进行绘图操作。
>
