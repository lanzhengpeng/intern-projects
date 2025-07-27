### 【2092】report face loops 报告面循环

#### 代码

```cpp
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    static void report_face_loops(tag_t face)  
    {  
        char  
            *types[4] = { "", "Peripheral", "Hole", "Other" };  
        int  
            ii,  
            jj,  
            n_edges,  
            n_loops,  
            type;  
        tag_t  
            edge;  
        uf_list_p_t  
            edge_list;  
        uf_loop_p_t  
            loop_list;  
        char  
            msg[133];  
        if (!UF_CALL(UF_MODL_ask_face_loops(face, &loop_list)) && loop_list)  
        {  
            UF_CALL(UF_MODL_ask_loop_list_count(loop_list, &n_loops));  
            sprintf(msg, "\nface %d has %d loops\n", face, n_loops);  
            WRITE(msg);  
            for (ii = 0; ii < n_loops; ii++)  
            {  
                UF_CALL(UF_MODL_ask_loop_list_item(loop_list,ii,&type,&edge_list));  
                UF_CALL(UF_MODL_ask_list_count(edge_list, &n_edges));  
                sprintf(msg, "  %d.  %s loop has %d edges\n", ii+1, types[type],  
                    n_edges);  
                WRITE(msg);  
                for (jj = 0; jj < n_edges; jj++)  
                {  
                    UF_CALL(UF_MODL_ask_list_item(edge_list, jj, &edge));  
                    sprintf(msg, "    %d.  edge %d\n", jj+1, edge);  
                    WRITE(msg);  
                }  
            }  
            UF_CALL(UF_MODL_delete_loop_list(&loop_list));  
        }  
    }

```

#### 代码解析

> 这段代码是NX Open C++的二次开发代码，用于报告NX模型中一个面(face)的环(loop)信息。具体来说，代码实现了以下功能：
>
> 1. 定义了一个宏WRITE，用于打开NX的列表窗口并写入信息。
> 2. 定义了一个静态函数report_face_loops，用于报告指定面的环信息。
> 3. 在函数中，首先使用UF_MODL_ask_face_loops获取面的环列表。
> 4. 遍历环列表，使用UF_MODL_ask_loop_list_count获取环的数量，然后逐个报告每个环的信息。
> 5. 对于每个环，使用UF_MODL_ask_loop_list_item获取环的类型和边列表。
> 6. 遍历边列表，使用UF_MODL_ask_list_count获取边的数量，然后逐个报告每条边的信息。
> 7. 最后，使用UF_MODL_delete_loop_list删除环列表。
> 8. 整个过程中，使用WRITE宏在NX的列表窗口中输出环和边的详细信息。
>
> 综上所述，这段代码通过遍历面中的环和边，并使用NX Open API查询相关信息，实现了对面环结构的查询和报告功能。
>
