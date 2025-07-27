### 【2471】rotate geometry about XC axis 关于XC轴旋转几何体

#### 代码

```cpp
    /*HEAD ROTATE_GEOMETRY_ABOUT_XC_AXIS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_csys.h>  
    #include <uf_trns.h>  
    #include <uf_vec.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是 V18 中的新增功能，仅需要回答译文，无需提供额外解释。 */  
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
    static int select_objects(char *prompt, tag_t **objects)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        tag_t  
            *objs;  
        UF_CALL(UF_UI_select_with_class_dialog( prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART, NULL, NULL, &resp, &cnt, objects));  
        objs = *objects;  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight(objs[ii], 0));  
        return cnt;  
    }  
    static logical prompt_for_a_number(char *prompt, char *item, double *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        double  
            da[1];  
        strcpy(&menu[0][0], item);  
        da[0] = *number;  
        resp = uc1609(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            one = 1,  
            n,  
            resp,  
            two = 2,  
            zero = 0;  
        tag_t  
            *objects,  
            mx,  
            wcs;  
        double  
            angle = 90.0,  
            origin[3],  
            rotation[12],  
            wcs_vectors[9];  
        while (((n = select_objects("Select objects to rotate", &objects)) > 0)  
            && (prompt_for_a_number("Rotation angle", "degrees", &angle)))  
        {  
            UF_CALL(UF_CSYS_ask_wcs(&wcs));  
            UF_CALL(UF_CSYS_ask_csys_info(wcs, &mx, origin));  
            UF_CALL(UF_CSYS_ask_matrix_values(mx, wcs_vectors));  
        /*  This will rotate about the XC axis - 1st 3 array positions 里海译:这将在XC轴周围旋转 - 前3个数组位置 */  
            FTN(uf5945)(origin, wcs_vectors, &angle, rotation, &resp);  
            FTN(uf5947)(rotation, objects, &n, &one,&zero,&two,NULL,NULL,&resp);  
            UF_free(objects);  
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

> 这段NX二次开发代码的主要功能是旋转几何体。以下是该代码的主要内容和功能：
>
> 1. 包含了必要的NX Open API头文件。
> 2. 定义了report_error函数，用于在API调用失败时打印错误信息。
> 3. 定义了select_objects函数，用于让用户选择要旋转的几何体，并高亮显示所选对象。
> 4. 定义了prompt_for_a_number函数，用于提示用户输入旋转角度。
> 5. do_it函数实现了旋转的主要逻辑。首先选择要旋转的几何体，然后提示输入旋转角度。接着获取当前工作坐标系的信息，并使用NX Open API的UF_CSYS_ask_wcs等函数计算旋转矩阵。最后调用UF_TRNS_rotate_objects函数实现旋转。
> 6. ufusr函数是NX二次开发的入口函数，在这里调用do_it函数实现旋转功能。
> 7. ufusr_ask_unload函数用于设置二次开发的卸载方式。
>
> 总的来说，这段代码实现了选择几何体、输入旋转角度，然后绕工作坐标系的XC轴旋转所选几何体的功能。通过二次开发，扩展了NX的几何操作功能。
>
