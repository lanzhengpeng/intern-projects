### 【1654】move objects from layer to layer 将对象从一个图层移动到另一个图层

#### 代码

```cpp
    /*HEAD MOVE_OBJECTS_FROM_LAYER_TO_LAYER CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_layer.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是 V18 新增的函数。 */  
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
    static void set_layer_active(int layer)  
    {  
            UF_CALL(UF_LAYER_set_status(layer, UF_LAYER_ACTIVE_LAYER));  
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
    static int ask_all_objects_on_layer(int layer, tag_t **objects)  
    {  
        tag_t  
            object = NULL_TAG;  
        uf_list_p_t  
            list;  
        UF_CALL(UF_MODL_create_list(&list));  
        while (!UF_CALL(UF_LAYER_cycle_by_layer(layer, &object)) &&  
            (object != NULL_TAG)) UF_CALL(UF_MODL_put_list_item(list, object));  
        return make_an_array(&list, objects);  
    }  
    #ifndef UF_LAYER_MAX_LAYER  
        #define UF_LAYER_MAX_LAYER 256  
    #endif  
    static logical prompt_for_a_layer(char *prompt, char *item, int *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            laymsg[100],  
            menu[1][16];  
        int  
            da[1];  
        strcpy(&menu[0][0], item);  
        da[0] = *number;  
        resp = uc1607(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            if ((*number > UF_LAYER_MAX_LAYER) || (*number <= 0))  
            {  
                sprintf(laymsg, "Layers range from 1 to %d", UF_LAYER_MAX_LAYER);  
                uc1601(laymsg, TRUE);  
                return prompt_for_a_layer(prompt, item, number);  
            }  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            ii = 0,  
            layer = 1,  
            layer_to = 1,  
            n_objs;  
        tag_t  
            *objs;  
        char  
            messg[133];  
        while (((prompt_for_a_layer("Move objects", "FROM Layer", &layer)) != 0) &&  
               ((prompt_for_a_layer("Move objects", "TO Layer", &layer_to)) != 0))  
        {  
                set_layer_active(layer);  
                /* uncomment the following to make destination layer visible 里海译:将以下内容取消注释，使目标层可见 */  
                /* set_layer_active(layer_to); 里海译:set_layer_active(layer_to); */  
                n_objs = ask_all_objects_on_layer(layer, &objs);  
                if (n_objs > 0)  
                {  
                    for (ii = 0; ii < n_objs; ii++)  
                    {  
                        UF_CALL(UF_OBJ_set_layer(objs[ii], layer_to));  
                    }  
                    UF_free(objs);  
                }  
                else  
                {  
                    sprintf(messg, "\nNo objects found on layer %d",layer);  
                    uc1601(messg,1);  
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

> 这段代码是NX的二次开发代码，实现了从源层向目标层移动对象的功能。
>
> 主要步骤包括：
>
> 1. 初始化NX环境。
> 2. 循环提示用户输入源层和目标层的编号。
> 3. 激活源层，获取源层上的所有对象，并保存到一个数组中。
> 4. 将数组中的每个对象移动到目标层。
> 5. 释放数组内存。
> 6. 如果源层没有对象，提示用户。
> 7. 重复步骤2-6，直到用户不再需要移动对象。
> 8. 结束NX环境。
>
> 关键点包括：
>
> 1. 使用UF_MODL_create_list、UF_LAYER_cycle_by_layer和UF_MODL_put_list_item获取源层上的所有对象。
> 2. 使用UF_OBJ_set_layer将对象移动到目标层。
> 3. 使用UF_LAYER_set_status激活层。
> 4. 使用UF_allocate_memory和UF_free管理内存。
> 5. 使用UF_print_syslog打印错误信息。
> 6. 使用uc1607和uc1601实现人机交互提示。
> 7. UF_CALL宏用于调用NX API并报告错误。
>
> 总的来说，这段代码实现了从源层向目标层移动对象的功能，并具有错误处理和人机交互提示。
>
