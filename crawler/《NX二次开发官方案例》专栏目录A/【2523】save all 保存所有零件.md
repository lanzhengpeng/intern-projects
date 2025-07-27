### 【2523】save all 保存所有零件

#### 代码

```cpp
    static void save_all(void)  
    {  
        int  
            count,  
            *error_list,  
            ii;  
        tag_t  
            *parts_list;  
        char  
            failure[133],  
            partname[UF_CFI_MAX_FILE_NAME_SIZE];  
        UF_CALL(UF_PART_save_all(&count, &parts_list, &error_list));  
        if (count > 0)  
        {  
            UF_UI_open_listing_window();  
            UF_UI_write_listing_window("Save notes:\n");  
            for (ii = 0; ii < count; ii++)  
            {  
                UF_PART_ask_part_name(parts_list[ii], partname);  
                UF_get_fail_message(error_list[ii], failure);  
                UF_UI_write_listing_window("  ");  
                UF_UI_write_listing_window(partname);  
                UF_UI_write_listing_window(" - ");  
                UF_UI_write_listing_window(failure);  
                UF_UI_write_listing_window("\n");  
            }  
            UF_free(parts_list);  
            UF_free(error_list);  
        }  
    }

```

#### 代码解析

> 这段NX二次开发代码的作用是保存所有零件。代码首先使用UF_PART_save_all函数尝试保存所有零件，并将结果存储在count、parts_list和error_list变量中。如果存在无法保存的零件，即count大于0，则打开一个列表窗口，并逐个显示无法保存的零件名称和错误信息。最后，释放分配的内存。总的来说，这段代码的目的是检查所有零件是否保存成功，并将失败的零件及其错误信息显示给用户。
>
