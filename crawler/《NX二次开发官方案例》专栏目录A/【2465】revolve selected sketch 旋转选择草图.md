### 【2465】revolve selected sketch 旋转选择草图

#### 代码

```cpp
    /*HEAD REVOLVE_SELECTED_SKETCH CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
    #include <uf_object_types.h>  
    #include <uf_vec.h>  
    #include <uf_eval.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档中的说明，UF_print_syslog是V18版本中新增的函数。对于问题“UF_print_syslog is new in V18”，翻译为“UF_print_syslog是在V18版本中新增的”。 */  
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
    static tag_t select_a_sketch(char *prompt)  
    {  
        int  
            resp;  
        tag_t  
            sketch;  
        if (!UF_CALL(UF_UI_select_sketch(prompt, NULL, &sketch, &resp))  
        && (resp == UF_UI_OK))  
           return sketch;  
        else  
            return NULL_TAG;  
    }  
    static logical specify_vector(char *prompt, double vec[3], double point[3])  
    {  
        int  
            mode = UF_UI_NO_DEFAULT,  
            resp;  
        UF_CALL(UF_UI_specify_vector(prompt, &mode, UF_UI_DISP_TEMP_VECTOR,  
            vec, point, &resp));  
        if (resp == UF_UI_OK) return TRUE;  
        else return FALSE;  
    }  
    static logical prompt_for_two_numbers(char *prompt, char *item1, char *item2,  
        double *number1, double *number2)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[2][16];  
        double  
            da[2];  
        strncpy(&menu[0][0], item1, 15);  
        strncpy(&menu[1][0], item2, 15);  
        da[0] = *number1;  
        da[1] = *number2;  
        resp = uc1609(prompt, menu, 2, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number1 = da[0];  
            *number2 = da[1];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            n_feats;  
        tag_t  
            *feats,  
            sketch;  
        uf_list_p_t  
            list,  
            feat_list;  
        char  
            e_angle[20],  
            *limits[2],  
            *offsets[2] = { "0", "0" },  
            s_angle[20];  
        double  
            dir[3],  
            end = 360,  
            point[3],  
            start = 0;  
        limits[0] = s_angle;  
        limits[1] = e_angle;  
        while (((sketch = select_a_sketch("Revolve")) != NULL_TAG)  
            && (specify_vector("Define revolution axis", dir, point))  
            && prompt_for_two_numbers("Limits", "Start Angle", "End Angle",  
                &start, &end))  
        {  
            sprintf(s_angle, "%f", start);  
            sprintf(e_angle, "%f", end);  
            UF_CALL(UF_MODL_create_list(&list));  
            UF_CALL(UF_MODL_put_list_item(list, sketch));  
            if (!UF_CALL(UF_MODL_create_revolved(list, limits, point, dir,  
                UF_NULLSIGN, &feat_list)))  
                UF_CALL(UF_MODL_delete_list(&feat_list));  
        /*  or  里海译:你好，请问有什么需要帮助的吗？ */  
            if (!UF_CALL(UF_MODL_create_revolution(&sketch, 1, NULL, limits,  
                offsets, point, FALSE, FALSE, point, dir, UF_NULLSIGN, &feats,  
                &n_feats)))  
                UF_free(feats);  
            UF_CALL(UF_MODL_delete_list(&list));  
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

> 这是一段NX的二次开发代码，主要实现了以下功能：
>
> 1. 错误报告函数：定义了一个错误报告函数report_error，用于在函数调用失败时报告错误信息。
> 2. 选择草图函数：定义了一个选择草图函数select_a_sketch，用于提示用户选择一个草图对象。
> 3. 指定向量函数：定义了一个指定向量函数specify_vector，用于提示用户指定一个旋转轴向量。
> 4. 提示输入两个数字函数：定义了一个提示输入两个数字函数prompt_for_two_numbers，用于提示用户输入起始角度和终止角度。
> 5. 主函数：主函数do_it中，循环提示用户选择草图、指定旋转轴和输入角度，然后调用UF_MODL_create_revolved或UF_MODL_create_revolution函数进行旋转操作，创建旋转特征。
> 6. UFusr函数：UFusr函数是NX的入口函数，初始化环境后调用do_it函数，最后终止环境。
> 7. 卸载函数：定义了一个卸载函数ufusr_ask_unload，用于在卸载NX时立即卸载二次开发代码。
>
> 这段代码实现了在NX中通过选择草图、指定旋转轴和输入角度来创建旋转特征的功能，具有一定的实用价值。
>
