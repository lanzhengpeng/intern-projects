### 【1830】reattach symbolic thread to selected faces 重新附着符号螺纹到选定面

#### 代码

```cpp
    /*HEAD REATTACH_SYMBOLIC_THREAD_TO_SELECTED_FACES CCC UFUN */  
    /*   
    Note:  
    UF_MODL_edit_symb_thread() allows to change the   
    inputs faces and direction as of NX8.5.1, see PR-8285563  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_vec.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[1025];  
        va_list args;  
        va_start(args, format);  
        vsnprintf_s(msg, sizeof(msg), 1024, format, args);  
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
    /* qq3123197280 */  
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
        UF_CALL(UF_UI_select_with_single_dialog("Select planar face", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART,  
            mask_for_planar_face, NULL, &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        return NULL_TAG;  
    }  
    static tag_t select_a_feature(char *prompt)  
    {  
        int  
            cnt,  
            resp;  
        tag_t  
            first,  
            *feats;  
        UF_CALL(UF_UI_select_feature(prompt, NULL, &cnt, &feats, &resp));  
        if (cnt)  
        {  
            first = feats[0];  
            UF_free(feats);  
            return (first);  
        }  
        else return (NULL_TAG);  
    }  
    static void report_symb_thread_data(UF_MODL_symb_thread_data_t *thread)  
    {  
        ECHO("Cylindrical face tag = %d\n", thread->cyl_face);  
        ECHO("Start face tag = %d\n", thread->start_face);  
        ECHO("Thread axis direction = (%f,%f,%f)\n",  
          thread->axis_direction[0],thread->axis_direction[1],  
          thread->axis_direction[2]);  
        ECHO("Number of starts = %d\n", thread->num_starts);  
        if (thread->include_instances == UF_MODL_INCL_INSTANCES)  
          ECHO("Include instances = Yes\n");  
        else  
          ECHO("Include instances = No\n");  
        if (thread->rotation == UF_MODL_RIGHT_HAND)  
          ECHO("Rotation = Right_hand\n");  
        else  
          ECHO("Rotation = Left_hand\n");  
        if (thread->length_flag == UF_MODL_FIXED_LENGTH)  
          ECHO("Thread length flag = Fixed length\n");  
        else  
          ECHO("Thread length flag = Full thread\n");  
        if (thread->tapered == UF_MODL_TAPERED)  
          ECHO("Thread tapered = Yes\n");  
        else  
          ECHO("Thread tapered = No\n");  
        ECHO("Length expression %s\n", thread->length);  
        ECHO("Form = %s\n", thread->form);  
        ECHO("Callout = %s\n", thread->callout);  
        ECHO("Method = %s\n", thread->method);  
        ECHO("Major diameter expression %s\n", thread->major_dia);  
        ECHO("Minor diameter expression %s\n", thread->minor_dia);  
        ECHO("Tapped diameter expression %s\n", thread->tapped_dia);  
        ECHO("Pitch expression %s\n", thread->pitch);  
        ECHO("Angle expression %s\n", thread->angle);  
    }  
    static void free_symb_thread_data(UF_MODL_symb_thread_data_t *thread)  
    {  
        if (thread->length) UF_free(thread->length);  
        if (thread->form) UF_free(thread->form);  
        if (thread->method) UF_free(thread->method);  
        if (thread->callout) UF_free(thread->callout);  
        if (thread->major_dia) UF_free(thread->major_dia);  
        if (thread->minor_dia) UF_free(thread->minor_dia);  
        if (thread->tapped_dia) UF_free(thread->tapped_dia);  
        if (thread->pitch) UF_free(thread->pitch);  
        if (thread->angle) UF_free(thread->angle);  
    }  
    static void null_symb_thread_data(UF_MODL_symb_thread_data_t *thread)  
    {  
        thread->length = NULL;  
        thread->form = NULL;  
        thread->method = NULL;  
        //thread->callout = NULL;  
        thread->major_dia = NULL;  
        thread->minor_dia = NULL;  
        thread->tapped_dia = NULL;  
        thread->pitch = NULL;  
        thread->angle = NULL;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            feat;  
        UF_MODL_symb_thread_data_t  
            thread;  
        int   
            ii,   
            type,   
            norm_dir;  
        double   
            point[3],  
            dir[3],  
            box[6],  
            radius,  
            rad_data,  
            dot;  
        while ((feat = select_a_feature("Select thread to edit")) != NULL_TAG)  
        {  
            if (!UF_CALL(UF_MODL_ask_symb_thread_parms(feat, &thread)))  
            {  
                report_symb_thread_data(&thread);  
                // we cannot 'null' the structure when changing faces and direction  
                //null_symb_thread_data(&thread);  
                if( (thread.cyl_face = select_a_cylindrical_face("Select a new cylindrical face")) != NULL_TAG &&   
                    (thread.start_face = select_planar_face("Select a new start face")) != NULL_TAG)  
                {  
                    // Make sure to have the correct thread axis as the new face axis may be different  
                    // We assume that the new thread will NOT be in reversed but similar direction  
                    // Hence, we measure the angle between the thread and new face axis  
                    UF_CALL( UF_MODL_ask_face_data(thread.cyl_face, &type, point, dir, box, &radius, &rad_data, &norm_dir) );  
                    UF_VEC3_dot(thread.axis_direction, dir, &dot);  
                    ECHO("DOT / cos(a): %f  ", dot);  
                    // The DOT product is equal to cos(a)  
                    // So if 0<DOT<1 then angle (a) is between 0 and 90 -> same orientation  
                    if( dot > 0 )   
                    {  
                        ECHO(" -> Previous thread axis and current face axis in same orientation.\n", dot);  
                        for(ii=0; ii<3; ii++) thread.axis_direction[ii] = dir[ii];  
                    }  
                    else  
                    {  
                        ECHO(" -> Previous thread axis and current face axis in opposite orientation - reversing.\n", dot);  
                        for(ii=0; ii<3; ii++) thread.axis_direction[ii] = -dir[ii];  
                    }  
                    report_symb_thread_data(&thread);  
                    UF_CALL(UF_MODL_edit_symb_thread(feat, &thread));  
                    UF_CALL(UF_MODL_update());  
                }  
                // UF_MODL_free_symb_thread_data introduced in NX10  
                UF_MODL_free_symb_thread_data(&thread);  
                // for earlier releases use our own function  
                //free_symb_thread_data(&thread);  
            }  
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

> 这段NX二次开发代码的主要功能是重新指定已创建的螺纹特征的面和方向。具体来说，该代码具有以下功能和特点：
>
> 1. 提供用户界面，允许用户选择要编辑的螺纹特征、圆柱面和起始面。
> 2. 读取当前螺纹的参数信息，包括圆柱面、起始面、轴线方向、螺距等。
> 3. 重新指定螺纹的圆柱面和起始面。
> 4. 检测新圆柱面的轴线方向，与当前螺纹轴线方向进行比较，以确定是否需要反转螺纹方向。
> 5. 调用UF_MODL_edit_symb_thread函数，根据用户的选择更新螺纹的参数。
> 6. 调用UF_MODL_update更新模型，以反映螺纹参数的更改。
> 7. 提供错误处理和日志输出功能。
> 8. 使用了UF_MODL_free_symb_thread_data函数来释放螺纹数据结构占用的内存，该函数在NX10及以上版本提供。
>
> 总体来说，该代码实现了选择、读取、更新和释放螺纹特征参数的功能，通过用户界面进行交互，并具有错误处理和内存管理功能。
>
