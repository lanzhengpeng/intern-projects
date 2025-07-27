### 【0513】compare two selected faces for equivalence 比较两个选定面是否等价

#### 代码

```cpp
    /*HEAD COMPARE_TWO_SELECTED_FACES_FOR_EQUIVALENCE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_modl.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    /* 里海 */  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，我翻译如下：

注：UF_print_syslog是在V18版本中新增的函数。 */  
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
    /* 里海 */  
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
    /* 里海 */  
    static void do_it(void)  
    {  
        int  
            relation = 1,       /*    1 = Geometric Equivalence     译:翻译为：几何等价性 */  
            equivalent = 0;  
        tag_t  
            face_1 = NULL_TAG,  
            face_2 = NULL_TAG;  
        char  
            msg[132] = { "" };  
        while( ( face_1 = select_a_face( "Select First Face" )) != NULL_TAG &&  
               ( face_2 = select_a_face( "Select Second Face" )) != NULL_TAG  )  
        {  
            UF_CALL( UF_MODL_compare( face_1, face_2, relation, &equivalent ));  
            if( equivalent )  
            {  
                strcpy( msg, "Selected Faces Are Equivalent" );  
            }  
            else  
            {  
                strcpy( msg, "Selected Faces Are NOT Equivalent" );  
            }  
            uc1601( msg, TRUE );  
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

> 这段代码是一个NX的二次开发示例，主要功能是比较两个选中的面是否几何等价。
>
> 主要内容包括：
>
> 1. 定义了一个错误报告函数report_error，用于在UF函数调用失败时打印错误信息。
> 2. 定义了一个面选择掩码函数mask_for_faces，用于设置选择面时只允许选择实体类型中的任意面。
> 3. 定义了一个选择面的函数select_a_face，用于弹出对话框让用户选择一个面，并返回面的tag。
> 4. 定义了主函数do_it，在循环中调用select_a_face选择两个面，然后调用UF_MODL_compare函数比较这两个面是否几何等价，并打印出比较结果。
> 5. 定义了ufusr函数，初始化UF模块后调用do_it函数，最后清理UF模块。
> 6. 定义了ufusr_ask_unload函数，用于在卸载时立即卸载。
>
> 整个代码实现了比较两个面是否几何等价的功能，通过UF_MODL_compare函数进行几何比较，并通过错误报告函数打印出错误信息，是一个完整的NX二次开发示例。
>
