### 【2445】reposition an object 重新定位一个对象

#### 代码

```cpp
    /*HEAD REPOSITION_AN_OBJECT CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_csys.h>  
    #include <uf_trns.h>  
    #include <uf_mtx.h>  
    #include <stdarg.h>  
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
    /* qq3123197280 */  
    static int mask_for_csys(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_coordinate_system_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_csys(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a coordinate system",  
            prompt, UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_csys, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void matrix_csys_to_csys(tag_t ref_csys, tag_t dest_csys, double mx[16])  
    {  
        double  
            from_csys[9],  
            from_orig[3],  
            to_csys[9],  
            to_orig[3];  
        tag_t  
            csys_mx;  
        UF_CALL(UF_CSYS_ask_csys_info(ref_csys, &csys_mx, from_orig));  
        UF_CALL(UF_CSYS_ask_matrix_values(csys_mx, from_csys));  
        UF_CALL(UF_CSYS_ask_csys_info(dest_csys, &csys_mx, to_orig));  
        UF_CALL(UF_CSYS_ask_matrix_values(csys_mx, to_csys));  
        UF_CALL(UF_MTX4_csys_to_csys(from_orig, from_csys, &from_csys[3],  
            to_orig, to_csys, &to_csys[3], mx));  
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
    static void do_it(void)  
    {  
        int  
            irc,  
            one = 1,  
            two = 2,  
            zero = 0;  
        tag_t  
            d_csys,  
            grp,  
            r_csys;  
        tag_p_t  
            datum;  
        double  
            mx[16];  
        while ((r_csys = select_a_csys("Reference csys")) &&  
               (d_csys = select_a_csys("Destination csys")) &&  
               ( select_objects("Select one object to reposition", &datum)))  
        {  
            matrix_csys_to_csys(r_csys, d_csys, mx);  
            FTN(uf5947)(mx, datum, &one, &one, &zero, &two, datum, &grp, &irc);  
            UF_CALL(irc);  
        }  
        UF_free(datum);  
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

> 这段代码是一个NX的二次开发代码，其主要功能是实现坐标系之间的转换，并将选定的对象从一个坐标系转换到另一个坐标系。具体来说，代码的主要功能包括：
>
> 1. 选择参考坐标系和目标坐标系。
> 2. 计算参考坐标系到目标坐标系的变换矩阵。
> 3. 选择要变换的对象。
> 4. 应用变换矩阵将选定的对象从参考坐标系变换到目标坐标系。
>
> 代码的主要步骤包括：
>
> 1. 定义错误报告函数report_error，用于输出错误信息。
> 2. 定义函数mask_for_csys，用于设置选择坐标系时的选择掩码。
> 3. 定义函数select_a_csys，用于通过对话框选择坐标系。
> 4. 定义函数matrix_csys_to_csys，用于计算两个坐标系之间的变换矩阵。
> 5. 定义函数select_objects，用于通过对话框选择要变换的对象。
> 6. 定义函数do_it，用于实现坐标系转换功能。该函数首先选择参考坐标系和目标坐标系，然后选择要变换的对象，接着计算变换矩阵，最后应用变换矩阵将对象从参考坐标系变换到目标坐标系。
> 7. 定义ufusr函数，作为程序的入口，初始化NX环境，调用do_it函数，最后清理环境。
> 8. 定义ufusr_ask_unload函数，用于处理程序卸载。
>
> 总体来说，这段代码通过用户交互选择坐标系和对象，并实现了坐标系之间的转换功能。
>
