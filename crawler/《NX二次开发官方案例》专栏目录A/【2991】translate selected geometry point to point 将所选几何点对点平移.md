### 【2991】translate selected geometry point to point 将所选几何点对点平移

#### 代码

```cpp
    /*HEAD TRANSLATE_SELECTED_GEOMETRY_POINT_TO_POINT CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_trns.h>  
    #include <uf_disp.h>  
    #include <uf_vec.h>  
    #define ECHO(X)    (printf("%s = %d\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本中新增的函数。在回答问题时，请确保只提供翻译，不要添加任何额外的内容。 */  
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
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, NULL, NULL, &resp, &cnt, objects));  
        objs = *objects;  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight(objs[ii], 0));  
        return cnt;  
    }  
    static logical specify_point(char *prompt, double loc[3])  
    {  
        int  
            ia2[2] = { 0,0 },  
            ip3 = 0,  
            irc;  
        irc = uc1616(prompt, ia2, ip3, loc);  
        if (irc == 5) return TRUE;  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            n,  
            off = 2,  
            orig = 0,  
            move = 1,  
            resp;  
        tag_t  
            *stuff;  
        double  
            delta[3],  
            dest[3],  
            ref[3],  
            mx[12];  
        while (((n = select_objects("Select objects to translate", &stuff)) > 0)  
            && specify_point("Indicate reference point", ref)  
            && specify_point("Indicate destination point", dest))  
        {  
            UF_VEC3_sub(dest, ref, delta);  
            FTN(uf5943)(delta, mx);  
            FTN(uf5947)(mx, stuff, &n, &move, &orig, &off, NULL, NULL, &resp);  
            ECHO(resp);  
            UF_free(stuff);  
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

> 这段代码是用于NX的二次开发，主要功能是平移选定的几何体。
>
> 主要步骤包括：
>
> 1. 包含所需的头文件，如UF.h、uf_trns.h等。
> 2. 定义错误报告函数report_error，用于打印出函数调用失败的错误信息。
> 3. 定义选择对象函数select_objects，用于通过对话框选择对象。
> 4. 定义指定点函数specify_point，用于通过提示指定一个点。
> 5. 在do_it函数中，首先通过select_objects函数选择对象，然后通过specify_point函数指定参考点和目标点。
> 6. 计算平移向量，并构造平移矩阵。
> 7. 调用uf5947函数进行平移操作。
> 8. 在ufusr函数中，初始化NX环境，调用do_it函数，然后终止NX环境。
> 9. 定义卸载函数，返回立即卸载。
>
> 该代码通过对话框选择要平移的几何体，然后指定参考点和目标点，计算平移向量，最后调用NX的平移函数进行平移。在出现函数调用错误时，会打印出错误信息。
>
