### 【1602】map occ2assy 将occurrence中的坐标映射到assembly坐标系

#### 代码

```cpp
    static void map_occ2assy(tag_t occ, double pos[3])  
    {  
        tag_t  
            old_work = UF_ASSEM_ask_work_part();  
        UF_CALL(UF_ASSEM_set_work_occurrence(UF_ASSEM_ask_part_occurrence(occ)));  
        UF_CALL(UF_CSYS_map_point(UF_CSYS_WORK_COORDS, pos, UF_CSYS_ROOT_COORDS,  
            pos));  
        UF_CALL(UF_ASSEM_set_work_part(old_work));  
    }

```

#### 代码解析

> 这段代码是用于NX二次开发的，其主要功能是将工作坐标系中的点映射到根坐标系中。代码的步骤如下：
>
> 1. 获取当前的工作零件(old_work)。
> 2. 设置工作坐标系为当前发生器(occ)对应的零件。
> 3. 使用UF_CSYS_map_point函数将工作坐标系中的点(pos)映射到根坐标系，结果仍存储在pos数组中。
> 4. 恢复之前的工作零件(old_work)。
>
> 通过这段代码，可以方便地将不同坐标系中的点进行转换，进而实现坐标系之间的映射。
>
