### 【2101】report face uv minmax 报告面UV最小最大值

#### 代码

```cpp
    /*HEAD REPORT_FACE_UV_MINMAX CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
    #define ECHO4F(X)  (printf("%s = %f, %f, %f, %f\n",#X,*X,*(X+1),*(X+2),*(X+3)))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本中新增的函数。 */  
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
    static int mask_for_faces(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_face(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faces, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void display_temporary_colored_point(double *coords, int color)  
    {  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        attrib.color = color;  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            coords, &attrib, UF_DISP_POINT));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            face;  
        double  
            junk[3],  
            parms[2],  
            point[3],  
            uvs[4];  
        while ((face = select_a_face("Select face")) != NULL_TAG)  
        {  
            UF_CALL(UF_MODL_ask_face_uv_minmax(face, uvs));  
            ECHO4F(uvs);  
        /*  Min U and Min V = Green 里海译:Min U 和 Min V = Green */  
            parms[0] = uvs[0];  
            parms[1] = uvs[2];  
            UF_CALL(UF_MODL_ask_face_props(face, parms, point, junk, junk, junk,  
                junk, junk, junk));  
            display_temporary_colored_point(point, UF_OBJ_GREEN);  
        /*  Mid U and Mid V = Yellow 里海译:Mid U and Mid V = Yellow */  
            parms[0] = (uvs[0] + uvs[1])/2;  
            parms[1] = (uvs[2] + uvs[3])/2;  
            UF_CALL(UF_MODL_ask_face_props(face, parms, point, junk, junk, junk,  
                junk, junk, junk));  
            display_temporary_colored_point(point, UF_OBJ_YELLOW);  
        /*  Max U and Max V = Red 里海译:翻译为：Max U 和 Max V = 红色 */  
            parms[0] = uvs[1];  
            parms[1] = uvs[3];  
            UF_CALL(UF_MODL_ask_face_props(face, parms, point, junk, junk, junk,  
                junk, junk, junk));  
            display_temporary_colored_point(point, UF_OBJ_RED);  
        }  
    }  
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

> 这段NX二次开发代码的主要功能如下：
>
> 1. 提供一个对话框，允许用户选择一个面实体。
> 2. 当选择一个面实体后，代码会获取该面的UV参数范围，即最小U、最大U、最小V和最大V。
> 3. 使用UF_MODL_ask_face_props函数，根据UV参数范围计算出面实体的三个关键点：最小U和最小V处的点、最大U和最大V处的点、以及中间点。
> 4. 在NX图形窗口中，使用不同的颜色（绿色、黄色和红色）显示这三个关键点。
> 5. 重复上述过程，直到用户没有选择新的面实体为止。
> 6. 提供错误报告机制，当函数调用出错时，会打印错误信息。
> 7. 定义了一个卸载函数，当用户关闭NX时，该函数会被调用，以便释放代码占用的资源。
>
> 总的来说，这段代码通过获取面的UV参数范围，并计算出关键点，使用户可以直观地看到面的UV布局，对曲面建模非常有用。
>
