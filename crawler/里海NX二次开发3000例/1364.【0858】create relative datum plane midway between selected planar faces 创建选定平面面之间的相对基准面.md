### 【0858】create relative datum plane midway between selected planar faces 创建选定平面面之间的相对基准面

#### 代码

```cpp
    /*HEAD CREATE_RELATIVE_DATUM_PLANE_MIDWAY_BETWEEN_SELECTED_PLANAR_FACES CCC UFUN */  
    /* The plane that this creates may be called a "bisector"   
       datum plane in interactive NX.   
       Note that the selected planes should lie parallel  
       to each other.  This will not work with perpendicular planes.  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_so.h>  
    #include <uf_modl.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    #define WRITENZ(X) if (X != 0) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    /* 里海 */  
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
        UF_CALL(UF_UI_select_with_single_dialog("Select a face", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faces, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            f1 = NULL_TAG,  
            f2 = NULL_TAG,  
            obj_eids[2],  
            dplanes[2];  
        int  
            num_dplanes = 0,  
            point_select[3] = { 0,0,0 },  
            which_plane = 1;  
        double  
            ref_point[3] = { 0.,0.,0. };  
        if( ((f1=select_a_face("FACE ONE")) != NULL_TAG) &&  
            ((f2=select_a_face("FACE TWO")) != NULL_TAG) )  
        {  
            obj_eids[0] = f1;  
            obj_eids[1] = f2;  
            UF_CALL(UF_MODL_create_relative_dplane(2, obj_eids, point_select,  
                      which_plane, ref_point, "0", "0", &num_dplanes, dplanes));  
            WRITE_D( num_dplanes );  
        }  
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

> 这段代码是用于在NX中创建一个相对于两个选择的面平面（datum plane）的二次开发代码。具体介绍如下：
>
> 1. 包含了必要的NX API头文件，并定义了错误报告函数report_error。
> 2. 定义了选择面的函数mask_for_faces和select_a_face，用于提示用户选择两个面。
> 3. do_it函数中，先调用select_a_face函数两次，让用户选择两个面。然后调用UF_MODL_create_relative_dplane函数，以这两个面为参照，创建一个相对于它们的平面。参数point_select指定平面通过哪个点，which_plane指定是相对于第一个面还是第二个面，ref_point指定平面的参考点。
> 4. ufusr函数是二次开发的入口函数，调用UF_initialize初始化NX API，调用do_it执行主体逻辑，然后调用UF_terminate终止NX API。
> 5. ufusr_ask_unload函数用于卸载二次开发，返回UF_UNLOAD_IMMEDIATELY表示立即卸载。
>
> 综上所述，这段代码实现了在NX中选择两个面，然后创建一个相对于这两个面的平面（datum plane）的功能。
>
