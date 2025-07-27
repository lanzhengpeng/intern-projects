### 【2993】translate selected geometry 选择并平移几何体

#### 代码

```cpp
    /*HEAD TRANSLATE_SELECTED_GEOMETRY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_trns.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的功能。 */  
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
    #define WRITENZ(X) if (X != 0) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
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
    static logical prompt_for_a_vector(char *prompt, double vec[3])  
    {  
        char  
            menu[3][16] = { "X", "Y", "Z" };  
        int  
            irc,  
            junk;  
        irc = uc1609(prompt, menu, 3, vec, &junk);  
        if (irc == 3 || irc == 4) return TRUE;  
        else return FALSE;  
    }  
    static uf_list_p_t make_a_list(int count, tag_t *item_array)  
    {  
        int  
            ii;  
        uf_list_p_t  
            list;  
        UF_CALL(UF_MODL_create_list(&list));  
        for (ii = 0; ii < count; ii++)  
            UF_CALL(UF_MODL_put_list_item(list, item_array[ii]));  
        return (list);  
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
            delta[3] = { 10.0, 0.0, 0.0 },  
            mx[12];  
        uf_list_p_t  
            object_list;  
        while (((n = select_objects("Select objects to translate", &stuff)) > 0)  
            && prompt_for_a_vector("Distances to translate", delta))  
        {  
            object_list = make_a_list(n, stuff);  
            UF_CALL(UF_MODL_delete_object_parms(object_list));  
            FTN(uf5943)(delta, mx);  
            FTN(uf5947)(mx, stuff, &n, &move, &orig, &off, NULL, NULL, &resp);  
            WRITENZ(resp);  
            UF_free(stuff);  
            UF_MODL_delete_list(&object_list);  
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

> 这段代码是一个NX的二次开发代码示例，主要用于实现以下功能：
>
> 1. 提示用户选择需要平移的物体，并获取用户输入的平移向量。
> 2. 将用户选择的物体创建成一个列表。
> 3. 删除列表中物体的参数，避免参数影响后续操作。
> 4. 计算平移矩阵。
> 5. 调用UF_TRNS模块的函数，根据用户输入的平移向量和计算出的平移矩阵，对物体进行平移。
> 6. 输出操作结果。
> 7. 重复上述操作，直到用户没有选择物体或输入平移向量。
> 8. 在操作过程中，使用宏函数报告错误，并在列表窗口中输出信息。
> 9. 用户可以随时卸载此程序。
>
> 这段代码实现了物体平移的二次开发功能，用户可以通过选择物体和平移向量来进行操作，代码结构清晰，易于理解和修改。
>
