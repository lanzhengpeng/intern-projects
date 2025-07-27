### 【0566】create a sketch 创建草图

#### 代码

```cpp
    /*HEAD CREATE_A_SKETCH CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_sket.h>  
    #define ECHO(X)    (printf("\t%s = %d\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，我翻译如下：

UF_print_syslog是在V18版本中新增的函数。 */  
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
    static int mask_for_planar_face_datum(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { { UF_solid_type, 0, UF_UI_SEL_FEATURE_PLANAR_FACE },  
                        { UF_datum_plane_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_planar_face_or_datum(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART,  
            mask_for_planar_face_datum, NULL, &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        return NULL_TAG;  
    }  
    static int mask_for_reference_object(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[4] = { { UF_solid_type, 0, UF_UI_SEL_FEATURE_PLANAR_FACE },  
                        { UF_datum_plane_type, 0, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_LINEAR_EDGE },  
                        { UF_datum_axis_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 4, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_reference_object(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART,  
            mask_for_reference_object, NULL, &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        return NULL_TAG;  
    }  
    static logical prompt_for_text(char *prompt, char *text)  
    {  
        int  
            n,  
            resp;  
        resp = uc1600(prompt, text, &n);  
        if (resp == 3 || resp == 5) return TRUE;  
        return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            reference[2] = { 1, 1 };  
        tag_t  
            objects[2],  
            sketch;  
        char  
            sket_name[MAX_ENTITY_NAME_SIZE+1] = { "SKETCH_000" };  
        while (((objects[0] = select_planar_face_or_datum(  
                "Select Attachment Face/Plane")) != NULL_TAG) &&  
            ((objects[1] = select_reference_object(  
                "Select Horizontal Reference")) != NULL_TAG) &&  
            prompt_for_text("Enter Sketch name", sket_name))  
        {  
            UF_CALL(UF_SKET_initialize_sketch(sket_name, &sketch));  
            UF_CALL(UF_SKET_create_sketch(sket_name, 1, NULL, objects, reference,  
                1, &sketch));  
            ECHO(sketch);  
            UF_CALL(UF_SKET_terminate_sketch());  
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

> 这段NX二次开发代码主要实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于在调用UF函数出错时打印错误信息。
> 2. 定义了两个选择函数mask_for_planar_face_datum和mask_for_reference_object，用于在对话框中过滤要选择的对象类型。
> 3. 定义了两个选择对象函数select_planar_face_or_datum和select_reference_object，用于让用户选择一个平面或基准面作为草图附着面，以及选择一个参考对象。
> 4. 定义了一个获取用户输入文本的函数prompt_for_text。
> 5. 定义了主函数do_it，在其中循环调用选择函数获取用户选择的附着面和参考对象，以及获取用户输入的草图名称，然后调用UF_SKET函数初始化草图环境，创建草图并结束草图环境。
> 6. 定义了ufusr函数，用于在NX启动时自动调用主函数do_it。
> 7. 定义了卸载函数ufusr_ask_unload，用于在卸载NX时返回立即卸载标志。
>
> 综上所述，这段代码实现了在NX中创建草图的功能，用户可以指定草图名称、附着面和参考对象，然后自动创建并结束草图。
>
