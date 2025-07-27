### 【0820】create plane from 3 points 从三个点创建平面

#### 代码

```cpp
    /*HEAD CREATE_PLANE_FROM_3_POINTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_curve.h>  
    #include <uf_mtx.h>  
    #include <uf_vec.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译如下：

注意：UF_print_syslog 是 V18 中的新增功能，请仅提供翻译，不要添加任何无关内容。 */  
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
    /* 里海 */  
    static int mask_for_points(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_point_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_point(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a point", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_points, NULL, &resp,  
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
            p1,  
            p2,  
            p3,  
            new_plane;  
        double  
            loc_1[3],  
            loc_2[3],  
            loc_3[3],  
            mtx[9],  
            vec1[3],  
            vec2[3];  
        while (((p1 = select_a_point( "First Point:" )) != NULL_TAG ) &&  
               ((p2 = select_a_point( "Second Point:" )) != NULL_TAG ) &&  
               ((p3 = select_a_point( "Third Point:" )) != NULL_TAG ))  
        {  
            UF_CALL( UF_CURVE_ask_point_data( p1, loc_1 ));  
            UF_CALL( UF_CURVE_ask_point_data( p2, loc_2 ));  
            UF_CALL( UF_CURVE_ask_point_data( p3, loc_3 ));  
            UF_VEC3_sub(loc_2, loc_1, vec1);  
            UF_VEC3_sub(loc_3, loc_1, vec2);  
            if (!UF_CALL(UF_MTX3_initialize(vec1, vec2, mtx)))  
                UF_CALL( UF_MODL_create_plane( loc_1, &mtx[6], &new_plane ));  
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

> 这段代码是一个NX Open C++的二次开发示例，主要用于从三个点创建平面。
>
> 1. 函数介绍ufusr：主函数，用于初始化NX环境，执行do_it函数，然后关闭NX环境。do_it：循环选择三个点，然后创建一个通过这三个点的平面。select_a_point：用于提示用户选择一个点，并返回点的tag。mask_for_points：用于设置选择掩码，只允许选择点。report_error：用于在出错时报告错误信息。
> 2. ufusr：主函数，用于初始化NX环境，执行do_it函数，然后关闭NX环境。
> 3. do_it：循环选择三个点，然后创建一个通过这三个点的平面。
> 4. select_a_point：用于提示用户选择一个点，并返回点的tag。
> 5. mask_for_points：用于设置选择掩码，只允许选择点。
> 6. report_error：用于在出错时报告错误信息。
> 7. 主要功能通过三个点创建平面。用户需要连续选择三个点，然后程序会使用这三个点创建一个平面。使用了NX提供的几何运算API，如UF_VEC3_sub计算向量，UF_MTX3_initialize初始化矩阵等。使用了NX选择API，通过对话框提示用户选择点。使用了错误处理机制，在出错时可以报告错误信息。
> 8. 通过三个点创建平面。用户需要连续选择三个点，然后程序会使用这三个点创建一个平面。
> 9. 使用了NX提供的几何运算API，如UF_VEC3_sub计算向量，UF_MTX3_initialize初始化矩阵等。
> 10. 使用了NX选择API，通过对话框提示用户选择点。
> 11. 使用了错误处理机制，在出错时可以报告错误信息。
> 12. 代码结构头文件包含NX Open C++的库。定义了错误处理宏UF_CALL。实现了选择点、创建平面的功能函数。实现了主函数ufusr。提供了卸载函数ufusr_ask_unload。
> 13. 头文件包含NX Open C++的库。
> 14. 定义了错误处理宏UF_CALL。
> 15. 实现了选择点、创建平面的功能函数。
> 16. 实现了主函数ufusr。
> 17. 提供了卸载函数ufusr_ask_unload。
>
> 总体来说，这段代码实现了选择三个点创建平面的功能，使用了NX提供的几何运算、选择等API，并具有完善的错误处理机制。
>
