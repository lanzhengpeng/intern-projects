### 【1971】report all spot weld data 报告所有点焊数据

#### 代码

```cpp
    /*HEAD REPORT_ALL_SPOT_WELD_DATA CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_weld.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据上下文，这句话的翻译是：UF_print_syslog是V18版本中新增的功能。 */  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[UF_UI_MAX_STRING_LEN+1],  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window(msg));  
            for (jj = 0; jj < cols; jj++)  
            {  
                sprintf(msg, "%f", array[kk++]);  
                UF_CALL(UF_UI_write_listing_window(msg));  
                if (jj < cols - 1)  
                {  
                    sprintf(msg, ", ");  
                    UF_CALL(UF_UI_write_listing_window(msg));  
                }  
            }  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window("\n"));  
        }  
    }  
    static tag_t ask_next_of_type(tag_t part, int type, tag_t object)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, type, &object));  
        return (object);  
    }  
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static int make_an_array(uf_list_p_t *object_list, tag_t **objects)  
    {  
        int  
            ii,  
            n;  
        uf_list_p_t  
            temp;  
        UF_CALL(UF_MODL_ask_list_count(*object_list, &n));  
        UF_CALL(allocate_memory(n * sizeof(tag_t), (void **)objects));  
        for (ii = 0, temp = *object_list; ii < n; temp = temp->next, ii++)  
            (*objects)[ii] = temp->eid;  
        UF_CALL(UF_MODL_delete_list(object_list));  
        return n;  
    }  
    static int ask_all_of_type(tag_t part, int type, tag_t **objects)  
    {  
        tag_t  
            object = NULL_TAG;  
        uf_list_p_t  
            object_list;  
        UF_CALL(UF_MODL_create_list(&object_list));  
        while ((object = ask_next_of_type(part, type, object)) != NULL_TAG)  
            UF_CALL(UF_MODL_put_list_item(object_list, object));  
        return (make_an_array(&object_list, objects));  
    }  
    static void report_object_name(char *name, tag_t object)  
    {  
        char  
            obj_name[UF_OBJ_NAME_LEN + 1];  
        printf("%s - tag %d is ", name, object);  
        if (!UF_OBJ_ask_name(object, obj_name)) printf("named %s\n", obj_name);  
        else printf("not named\n");  
    }  
    static void report_spot_weld_data(tag_t spot)  
    {  
        int  
            ii, jj,  
            n_sets,  
            n_spots;  
        UF_WELD_spot_info_t  
            *spot_info;  
        UF_CALL(UF_DISP_set_highlight(spot, TRUE));  
        UF_CALL(UF_WELD_ask_spot_face_data(spot, &n_sets, &n_spots, &spot_info));  
        WRITE_D(n_sets);  
        WRITE_D(n_spots);  
        for (ii = 0; ii < n_spots; ii++)  
        {  
            WRITE_D(spot_info[ii].spot_point);  
            for (jj = 0; jj < n_sets; jj++)  
            {  
                report_object_name("spot_info[ii].face_loc[jj].face_tag",  
                    spot_info[ii].face_loc[jj].face_tag);  
                UF_CALL(UF_DISP_set_highlight(spot_info[ii].face_loc[jj].face_tag,  
                    TRUE));  
                WRITE3F(spot_info[ii].face_loc[jj].point);  
                WRITE3F(spot_info[ii].face_loc[jj].face_normal);  
            }  
        }  
        UF_CALL(UF_WELD_free_spot_data(n_spots, &spot_info));  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_feats;  
        tag_t  
            *feats,  
            part = UF_PART_ask_display_part();  
        char  
            *type;  
        n_feats = ask_all_of_type(part, UF_feature_type, &feats);  
        for (ii = 0; ii < n_feats; ii++)  
        {  
            UF_CALL(UF_MODL_ask_feat_type(feats[ii], &type));  
            if (!strcmp(type, "SPOT_WELD")) report_spot_weld_data(feats[ii]);  
            UF_free(type);  
        }  
        if (n_feats > 0) UF_free(feats);  
        uc1601("Spot welds and associated faces highlighted", TRUE);  
        UF_CALL(UF_PART_cleanup(UF_PART_cleanup_highlight));  
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

> 这段代码是一个NX Open C++ API的二次开发示例，其主要功能是遍历当前部件中的所有特征，并高亮显示所有的点焊特征及其关联的面。
>
> 代码的主要逻辑如下：
>
> 1. 定义了一些辅助函数，如report_error用于报告错误、WRITE_D和WRITE3F用于向列表窗口打印整数和浮点数数组、ask_next_of_type用于遍历部件中指定类型的对象、allocate_memory用于分配内存、make_an_array用于创建数组、ask_all_of_type用于获取部件中指定类型的所有对象、report_object_name用于打印对象名称、report_spot_weld_data用于报告点焊数据。
> 2. 定义了do_it函数，用于遍历当前部件的所有特征，如果特征类型为“SPOT_WELD”，则调用report_spot_weld_data函数报告该点焊特征的数据，并高亮显示点焊特征和关联的面。
> 3. 定义了ufusr函数，作为程序的入口点。在初始化NX Open C++ API之后，调用do_it函数执行主体逻辑，最后进行清理工作。
> 4. 定义了ufusr_ask_unload函数，用于卸载用户自定义程序。
>
> 总体来说，这段代码通过NX Open C++ API遍历当前部件中的点焊特征，并高亮显示点焊位置和关联的面，以便进行进一步的检查和分析。
>
