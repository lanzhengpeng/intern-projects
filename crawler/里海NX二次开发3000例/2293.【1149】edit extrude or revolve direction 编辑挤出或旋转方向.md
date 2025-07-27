### 【1149】edit extrude or revolve direction 编辑挤出或旋转方向

#### 代码

```cpp
    /*HEAD EDIT_EXTRUDE_OR_REVOLVE_DIRECTION CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是 V18 中新增的函数。 */  
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
    static void display_conehead(double origin[3], double vec[3])  
    {  
        UF_DISP_conehead(UF_DISP_ALL_ACTIVE_VIEWS, origin, vec, 0);  
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
    static void do_it(void)  
    {  
        tag_t  
            feat;  
        double  
            dir[3],  
            loc[3];  
        while ((feat = select_a_feature("Modify extrude direction")) != NULL_TAG)  
        {  
        /*  This may not work - See PR 1095526 - fixed in V18.0.5.1 里海译:翻译如下：

这可能不起作用 - 请参阅PR 1095526 - 在V18.0.5.1版本中修复。 */  
            if (UF_CALL(UF_MODL_ask_sweep_direction(feat, loc, dir))) continue;  
            display_conehead(loc, dir);  
            if (specify_vector("New direction", dir, loc))  
            {  
            /*  This does not work until NX 1.0 - See PR 4745754 里海译:翻译：这方法在NX 1.0之前不起作用，请查看PR 4745754。 */  
                UF_CALL(UF_MODL_set_sweep_axis(feat, dir));  
            /*  This is necessary until NX 2.0 to get the feature to update 里海译:这是必要的，直到 NX 2.0 版本才能获得更新功能。 */  
                UF_CALL(UF_MODL_edit_sweep_curves(feat, 0, NULL, 0, NULL, 0, NULL,  
                    0, NULL));  
                UF_CALL(UF_MODL_update());  
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

> 这段代码是NX的二次开发代码，主要功能是修改特征的拉伸方向。
>
> 代码的主要流程如下：
>
> 1. 定义了一个错误报告函数report_error，用于在调用UF函数出错时打印错误信息。
> 2. 定义了一个函数select_a_feature，用于选择一个特征。
> 3. 定义了一个函数display_conehead，用于显示拉伸方向箭头。
> 4. 定义了一个函数specify_vector，用于指定一个新的方向向量。
> 5. 在主函数do_it中，循环调用select_a_feature选择特征，然后调用UF_MODL_ask_sweep_direction获取当前特征的拉伸方向，并显示。然后调用specify_vector让用户指定新的方向，如果用户指定了方向，就调用UF_MODL_set_sweep_axis设置新的方向，并调用UF_MODL_edit_sweep_curves和UF_MODL_update更新特征。
> 6. 最后定义了ufusr函数作为程序的入口，初始化NX环境，调用do_it执行主要逻辑，然后终止NX环境。
> 7. 定义了ufusr_ask_unload函数，用于在卸载用户程序时立即卸载。
>
> 总的来说，这段代码通过交互式地选择特征，获取并修改其拉伸方向，实现了特征拉伸方向的二次开发功能。
>
