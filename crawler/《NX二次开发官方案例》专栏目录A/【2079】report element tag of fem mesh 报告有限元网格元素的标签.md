### 【2079】report element tag of fem mesh 报告有限元网格元素的标签

#### 代码

```cpp
    static void report_element_tag_of_fem_mesh()  
    {  
        int  
            i,  
            num_of_ids = 7, /*num_of_ids has to be equal or less than the amount  
                               of existing elements*/  
            element_ids[7]={1, 2, 3, 4, 5, 6, 7}, /*array size should correlate to  
                               num_of_ids*/  
            num_of_elements;  
        char   
            msg[300] = { "" };  
        tag_t  
            mesh_tag = NULL_TAG,  
            *element_tags_p;  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "number of ids: %d\n", num_of_ids);  
        UF_CALL(UF_UI_write_listing_window ( msg ));  
        UF_CALL(UF_SF_locate_element_by_id(mesh_tag, num_of_ids, element_ids,  
                 &num_of_elements, &element_tags_p));  
        for(i=0 ; i<num_of_ids; i++)  
        {  
            sprintf(msg, "element_id %d has element tag: %d\n", element_ids[i], element_tags_p[i]);  
            UF_CALL(UF_UI_write_listing_window ( msg ));   
        }  
        UF_free(element_tags_p);     
    }

```

#### 代码解析

> 这段NX二次开发代码的主要功能是查询并报告有限元网格中指定元素ID对应的元素标签。
>
> 代码的主要步骤包括：
>
> 1. 定义了要查询的元素ID数组element_ids，以及元素ID的数量num_of_ids。
> 2. 调用UF_SF_locate_element_by_id函数，传入元素ID数组，获取对应的元素标签数组element_tags_p，以及实际找到的元素数量num_of_elements。
> 3. 通过循环遍历element_ids数组，利用sprintf函数格式化输出每个元素ID及其对应的元素标签到列表窗口。
> 4. 释放element_tags_p数组占用的内存。
> 5. 整个过程通过UF_UI_open_listing_window打开列表窗口，UF_UI_write_listing_window写入信息，UF_free释放内存等UF函数实现。
>
> 该代码的作用是查询有限元网格中指定元素ID的元素标签，并将查询结果输出到列表窗口，以便进行查看。
>
