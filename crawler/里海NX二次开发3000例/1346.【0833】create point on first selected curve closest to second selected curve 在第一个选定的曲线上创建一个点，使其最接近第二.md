### 【0833】create point on first selected curve closest to second selected curve 在第一个选定的曲线上创建一个点，使其最接近第二

#### 代码

```cpp
    /*****************************************************************************  
    **  
    ** Create a point on the first selected curve, at the location  
    **  
    ** that is closest to the second selected curve.   
    **  
    *****************************************************************************/  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_disp.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_curve.h>  
    #include <uf_modl.h>  
    #include <uf_part.h>  
    #include <uf_text.h>  
    #include <uf_ui.h>  
    #include <stdarg.h>  
    #pragma warning(disable:4996)  
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
    static int mask_for_curves(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[4] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, 0, 0 },  
                        { UF_spline_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 4, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    extern tag_t select_a_curve(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a curve: ", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_curves, NULL, &resp,  
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
        tag_t  
            thePart = NULL_TAG,  
            c1 = NULL_TAG,  
            c2 = NULL_TAG,  
            newPoint = NULL_TAG;  
        double  
            minimum_distance,  
            accuracy,  
            pt1[3],  
            pt2[3];  
        thePart = UF_PART_ask_display_part();  
        if (NULL_TAG == thePart)  
        {  
            ECHO("Please load a part and try again.\n");  
            return;  
        }  
        c1 = select_a_curve("FIRST CURVE:");  
        c2 = select_a_curve("SECOND CURVE:");  
        if (NULL_TAG == c1 || NULL_TAG == c2)  
        {  
            ECHO("You must pick each curve.\n");  
            return;  
        }  
        // ask minimum distances lets us find the point on each object that  
        // is the closest to the other object.  
        if (!UF_CALL(UF_MODL_ask_minimum_dist_3(2, c1, c2, FALSE, NULL,  
            FALSE, NULL, &minimum_distance, pt1, pt2, &accuracy)))  
        {  
            // pt1 is the minimum distance Point on the first object  
            UF_CALL(UF_CURVE_create_point(pt1, &newPoint));  
            ECHO("New point created at %f  %f  %f \n", pt1[0], pt1[1], pt1[2]);  
        }  
    }  
    /* 里海 */  
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

> 这段代码用于在NX中创建一个新点，该点位于第一条曲线且与第二条曲线最接近的位置。主要功能包括：
>
> 1. 选择两条曲线。使用自定义函数select_a_curve()，通过对话框提示用户选择两条曲线。
> 2. 计算两条曲线之间的最小距离。调用UF_MODL_ask_minimum_dist_3()函数，获取两条曲线之间的最小距离点。
> 3. 在第一条曲线上创建一个新点。使用UF_CURVE_create_point()函数，在第一条曲线上距离第二条曲线最近的点创建一个新点。
> 4. 输出新点的坐标。通过ECHO宏输出新点的坐标信息。
> 5. 错误处理。使用UF_CALL宏来捕获函数调用错误，并输出错误信息。
> 6. 初始化和终止NX API。在ufusr()函数中初始化和终止NX API。
> 7. 用户自定义函数。包括mask_for_curves()用于设置选择掩码，report_error()用于报告错误。
> 8. 代码中包含了中文注释，并使用了中文变量名，如"里海"。
>
> 以上是该代码的主要功能和流程，通过交互式选择曲线并计算最接近点，实现了在第一条曲线上创建新点的功能。
>
