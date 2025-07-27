### 【0551】copy symb threads 复制螺纹特征

#### 代码

```cpp
    /*HEAD COPY_SYMB_THREADS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档中的内容，UF_print_syslog是一个新函数，在V18版本中引入。该函数用于打印系统日志。 */  
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
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_cylindrical_faces, NULL,   
            &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static int mask_for_planar_face(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_PLANAR_FACE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_planar_face(char *prompt)  
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
            mask_for_planar_face, NULL, &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        return NULL_TAG;  
    }  
    static void report_symb_thread_data(UF_MODL_symb_thread_data_t *thread)  
    {  
        int  
            sub,  
            type;  
        UF_CALL(UF_OBJ_ask_type_and_subtype(thread->cyl_face, &type, &sub));  
        printf("\nCylindrical face tag = %d, type %d, subtype %d\n",  
            thread->cyl_face, type, sub);  
        UF_CALL(UF_OBJ_ask_type_and_subtype(thread->start_face, &type, &sub));  
        printf("Start face tag = %d, type %d, subtype %d\n",  
            thread->start_face, type, sub);  
        printf("Thread axis direction = (%f,%f,%f)\n",  
            thread->axis_direction[0], thread->axis_direction[1],  
            thread->axis_direction[2]);  
        printf("Number of starts = %d\n", thread->num_starts);  
        if (thread->include_instances == UF_MODL_INCL_INSTANCES)  
            printf("Include instances = Yes\n");  
        else  
            printf("Include instances = No\n");  
        if (thread->rotation == UF_MODL_RIGHT_HAND)  
            printf("Rotation = Right_hand\n");  
        else  
            printf("Rotation = Left_hand\n");  
        if (thread->length_flag == UF_MODL_FIXED_LENGTH)  
            printf("Thread length flag = Fixed length\n");  
        else  
            printf("Thread length flag = Full thread\n");  
        if (thread->tapered == UF_MODL_TAPERED)  
            printf("Thread tapered = Yes\n");  
        else  
            printf("Thread tapered = No\n");  
        printf("Length expression %s\n", thread->length);  
        printf("Form = %s\n", thread->form);  
        printf("Callout = %s\n", thread->callout);  
        printf("Method = %s\n", thread->method);  
        printf("Major diameter expression %s\n", thread->major_dia);  
        printf("Minor diameter expression %s\n", thread->minor_dia);  
        printf("Tapped diameter expression %s\n", thread->tapped_dia);  
        printf("Pitch expression %s\n", thread->pitch);  
        printf("Angle expression %s\n", thread->angle);  
    }  
    static void fixup_symb_thread_exps(UF_MODL_symb_thread_data_t *thread)  
    {  
        strcpy(thread->length, strchr(thread->length, '=') + 1);  
        strcpy(thread->major_dia, strchr(thread->major_dia, '=') + 1);  
        strcpy(thread->minor_dia, strchr(thread->minor_dia, '=') + 1);  
        strcpy(thread->tapped_dia, strchr(thread->tapped_dia, '=') + 1);  
        strcpy(thread->pitch, strchr(thread->pitch, '=') + 1);  
        strcpy(thread->angle, strchr(thread->angle, '=') + 1);  
    }  
    static void free_symb_thread_data(UF_MODL_symb_thread_data_t *thread)  
    {  
        UF_free(thread->length);  
        UF_free(thread->form);  
        UF_free(thread->method);  
        UF_free(thread->callout);  
        UF_free(thread->major_dia);  
        UF_free(thread->minor_dia);  
        UF_free(thread->tapped_dia);  
        UF_free(thread->pitch);  
        UF_free(thread->angle);  
    }  
    static void do_it(void)  
    {  
        int  
            n,  
            resp;  
        tag_t  
            feat,  
            *feats;  
        UF_MODL_symb_thread_data_t  
            thread;  
        while (!UF_UI_select_feature("Select thread to copy", NULL, &n, &feats,  
                 &resp) && (resp == UF_UI_OK) &&  
               (!UF_CALL(UF_MODL_ask_symb_thread_parms(feats[0], &thread))) &&  
               ((thread.cyl_face = select_a_cylindrical_face(  
                    "Select a cylindrical face")) != NULL_TAG) &&  
               ((thread.start_face = select_planar_face("Select start face"))  
                    != NULL_TAG))  
        {  
            fixup_symb_thread_exps(&thread);  
            report_symb_thread_data(&thread);  
            UF_CALL(UF_MODL_create_symb_thread(&thread, &feat));  
            UF_free(feats);  
            free_symb_thread_data(&thread);  
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

> 这段代码是NX的二次开发代码，其主要功能是复制螺纹特征。以下是代码的主要组成部分：
>
> 1. 头文件：包含了一些NX二次开发常用的库，如uf.h, uf_ui.h等。
> 2. 错误报告函数：用于在代码执行出错时报告错误信息。
> 3. 选择面函数：提供了两个函数，用于选择圆柱面和选择平面面。
> 4. 螺纹参数结构体：定义了螺纹的参数，如圆柱面、起始面、螺纹轴方向、螺距等。
> 5. 打印螺纹参数函数：用于打印出螺纹参数信息。
> 6. 修正螺纹参数函数：用于修正一些参数表达式的格式。
> 7. 释放螺纹参数内存函数：用于释放螺纹参数结构体内存。
> 8. 主函数：用于选择螺纹特征，获取参数，并复制螺纹。
> 9. ufusr函数：是NX二次开发的入口函数，初始化NX环境并调用主函数。
> 10. 卸载函数：用于卸载NX二次开发库。
>
> 总的来说，这段代码实现了选择现有螺纹，获取其参数，并创建一个新螺纹的功能。通过修正参数，它可以适应不同的螺纹规格。
>
