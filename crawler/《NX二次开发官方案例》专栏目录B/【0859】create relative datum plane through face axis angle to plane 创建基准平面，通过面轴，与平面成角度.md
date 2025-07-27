### 【0859】create relative datum plane through face axis angle to plane 创建基准平面，通过面轴，与平面成角度

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
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
    /* 里海 */  
    static int mask_for_cylindrical_faces(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_CYLINDRICAL_FACE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_cylindrical_face(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select cylindrical face", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_cylindrical_faces, NULL,  
            &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    /* 里海 */  
    static int mask_for_planar_object(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[3] = { { UF_solid_type, 0, UF_UI_SEL_FEATURE_PLANAR_FACE },  
                        { UF_plane_type, 0, 0 },  
                        { UF_datum_plane_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 3, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_planar_object(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select planar object", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART_AND_OCC, mask_for_planar_object, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        int  
            point_select[3] = { 0,0,0 },  
            n_planes;  
        tag_t  
            dplanes[2],  
            objects[3];  
        double  
            reference_point[3] = { 0,0,0 };  
    //  The order of the constraints is important - see PR 1902654.  
        objects[0] = select_a_planar_object("Angle to plane");  
        objects[1] = select_a_cylindrical_face("Through face axis");  
        UF_CALL(UF_MODL_create_relative_dplane(2, objects, point_select, 3,  
            reference_point, "90", "0", &n_planes, dplanes));  
    }  
    /* 里海 */  
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

> 这段NX二次开发代码的主要功能是创建一个相对基准平面。
>
> 代码的主要部分包括：
>
> 1. 定义了ECHO宏用于输出信息到日志窗口。
> 2. 定义了UF_CALL宏，用于调用NX API函数，并捕获错误。
> 3. 定义了mask_for_cylindrical_faces和mask_for_planar_object函数，用于设置选择面时的过滤条件。
> 4. 定义了select_a_cylindrical_face和select_a_planar_object函数，用于选择圆柱面和平面对象。
> 5. 在do_it函数中，首先调用select_a_planar_object选择一个平面，然后调用select_a_cylindrical_face选择一个圆柱面。最后调用UF_MODL_create_relative_dplane API函数，根据这两个对象创建一个相对基准平面。
> 6. 在ufusr函数中，初始化NX系统，调用do_it函数执行创建基准平面的操作，然后终止NX系统。
> 7. ufusr_ask_unload函数用于卸载用户自定义程序。
>
> 总体来说，这段代码实现了通过选择一个平面和一个圆柱面，在NX中创建一个相对基准平面的功能。
>
