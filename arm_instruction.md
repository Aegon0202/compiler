- `enum` 
  
  -   `IMMI_8` 8位立即数
  -   `IMMI_16` 16位立即数
  -   `REGISTER` 寄存器
  
    
  
- `struct Immi_8` 8位立即数
  
  -   `int8_t`  `:`  `num` 立即数
  
    
  
-  `struct Immi_12` 12位立即数

  -  `int16_t`  `:`  `num` 立即数



- `struct Immi_16` 16位立即数
  -   `int16_t`  `:`  `num` 立即数



-  `struct Register`  `ARM` 中的寄存器
  -   `int`  `:`  `reg` 寄存器编号从 `R0`  `~`  `R15` 
  



- `struct Operand2` 灵活的操作数2:
  -  `int`  `:`  `type` 操作数类型：
    -  `IMMI_8` 8位立即数，此时 `shift_op` 需为 `NOP` 
    -  `REGISTER` 寄存器
  - `union` `:`  `Rm` 操作数:
    - `struct Immi_8 *`  `:`  `immi_8` 8位立即数
    - `int`  `:`  `reg` 寄存器编号
  -  `int`  `:`  `shift_type` 移位操作:
    -  `LSL` 逻辑左移，允许移动 `0`  `~`  `31` 位
    -  `LSR` 逻辑右移，允许移动 `1`  `~`  `32` 位
    -  `ASR` 算数右移，允许移动 `1`  `~`  `32` 位
    -  `ROR` 循环右移，允许移动 `1`  `~`  `32` 位
    -  `RRX` 带扩展的循环右移一位，没有 `shift_op` 和 `shift` 
  -  `int`  `:`  `shift_op` 移位数据类型
    -  `IMMI_8` 立即数
    -  `REGISTER` 寄存器
  - `union` `:`  `shift` 移位操作数:
    - `struct Immi_8 *`  `:`  `immi_8` 满足移位操作限制位数的常量
    - `int`  `:`  `reg` 寄存器编号



-  `struct Label`  标签
  -  `char *`  `:`  `label` 标签内容



-  `struct CondOp` 条件字段，使指令被条件执行。大部分 `ARM` 指令支持在指令助记符后添加:
  -  `int`  `:`  `type` 条件字段:
    -  `EQ` 等于
    -  `NE` 不等于
    -  `HI` 无符号大于
    -  `LS` 无符号小于或等于
    -  `GE` 有符号大于或等于
    -  `LT` 有符号小于
    -  `GT` 有符号大于
    -  `LE` 有符号小于或等于
    -  `AL` 始终执行（通常省略）



-  `struct ArmIr` 与 `ARM` 指令有对应关系
  -  `int`  `:`  `type` 指令类型
  -  `struct CondOp *`  `:`  `cond` 指令的条件字段，不支持的为NULL
  -  `void *`  `:`  `op1` 第一个操作数
  -  `void *`  `:`  `op2` 第二个操作数
  -  `void *`  `:`  `op3` 第三个操作数
  -  `void *`  `:`  `op4` 第四个操作数



| `ArmIr`  `->`  `type`                                        | `op1`               | `op2`               | `op3`               | `op4`             |
| ------------------------------------------------------------ | ------------------- | ------------------- | ------------------- | ----------------- |
| `ARM_ADD`  `op1`  `<-`  `op2`  `+`  `op3`                    | `struct Register *` | `struct Register *` | `struct Operand2 *` | `NULL`            |
| `ARM_SUB`  `op1`  `<-`  `op2`  `-`  `op3`                    | `struct Register *` | `struct Register *` | `struct Operand2 *` | `NULL`            |
| `ARM_MUL`  `op1`  `<-`  `op2`  `*`  `op3`                    | `struct Register *` | `struct Register *` | `struct Register *` | `NULL`            |
| `ARM_SMULL`  `op2`  `:` `op1`  `<-`  `op3`  `*`  `op4` <br/>将乘的结果的低32位放到 `op1` 高32位放到 `op2` | `struct Register *` | `struct Register *` | `struct Register *` | `struct Register` |
| `ARM_SMMUL`  `op1`  `<-`  `(`  `op2`  `*`  `op3`  `)`  `[`  `64`  `:`  `32`  `]` <br/>将高32位的结果放到op1 | `struct Register *` | `struct Register *` | `struct Register *` | `NULL`            |
| `ARM_MOV`  `op1`  `<-` `op2`                                 | `struct Register *` | `struct Operand *`  | `NULL`              | `NULL`            |
| `ARM_MVN`  `op1`  `<-`  `0xFFFFFFFF`  `EOR`  `op2`           | `struct Register *` | `struct Operand *`  | `NULL`              | `NULL`            |
| `ARM_MOVT`  `op1`  `[`  `31`  `:`  `16`  `]`  `<-`  `op2` <br/> `op1`  `[`  `15`  `:`  `0`  `]` 不受影响<br/> `op2` 的范围为 `0`  `~`  `65535` | `struct Register *` | `struct Immi_16 *`  | `NULL`              | `NULL`            |
| `ARM_MOVW`  `op1`  `[`  `15`  `:`  `0`  `]`  `<-`  `op2` <br/> `op1`  `[`  `31`  `:`  `16`  `]`  `<-`  `0` <br/> `op2` 的范围为 `0`  `~`  `65535` | `struct Register *` | `struct Immi_16 *`  | `NULL`              | `NULL`            |
| `ARM_MOVT_L`  `op1`  `[`  `31`  `:`  `16`  `]`  `<-`  `op2` <br/> `op1`  `[`  `15`  `:`  `0`  `]` 不受影响<br/> `op2` 为标签的高 `16` 位地址，由汇编器实现。<br/> `#:upper:label` | `struct Register *` | `struct Label *`    | `NULL`              | `NULL`            |
| `ARM_MOVW_L`  `op1`  `[`  `15`  `:`  `0`  `]`  `<-`  `op2` <br/> `op1`  `[`  `31`  `:`  `16`  `]`  `<-`  `0` <br/> `op2` 为标签的低 `16` 位地址，由汇编器实现。<br/> `#:lower:label` | `struct Register *` | `struct Label *`    | `NULL`              | `NULL`            |
| `ARM_CLZ`  `op1`  `<-`  `op2` 中前导0的数目                  | `struct Register *` | `struct Register *` | `NULL`              | `NULL`            |
| `ARM_CMP` 更新 `op1`  `-`  `op2` 的 `CPSR` 标记              | `struct Register *` | `struct Operand *`  | `NULL`              | `NULL`            |
| `ARM_CMN` 更新 `op1`  `+`  `op2` 的 `CPSR` 标记              | `struct Register *` | `struct Operand *`  | `NULL`              | `NULL`            |
| `ARM_B`  `PC`  `<-`  `op1` <br/> `label` 为此指令 $\pm$  `32`  `MB ` | `struct Label *`    | `NULL`              | `NULL`              | `NULL`            |
| `ARM_B`  `PC`  `<-`  `op1` <br/> `LR` `<-`  下一条指令的地址<br/> `label` 为此指令 $\pm$  `32`  `MB ` | `struct Label *`    | `NULL`              | `NULL`              | `NULL`            |
| `ARM_BX`  `PC`  `<-`  `op1` <br/>如果 `op1`  `[`  `0`  `]` 为 `0` ，目标为 `ARM` <br/>如果 `op1`  `[`  `0`  `]` 为 `1` ，目标为 `Thumb` | `struct Register *` | `NULL`              | `NULL`              | `NULL`            |
| `ARM_BLX`  `PC`  `<-`  `op1` <br/> `LR` `<-`  下一条指令的地址<br/>如果 `op1`  `[`  `0`  `]` 为 `0` ，目标为 `ARM` <br/>如果 `op1`  `[`  `0`  `]` 为 `1` ，目标为 `Thumb` | `struct Register *` | `NULL`              | `NULL`              | `NULL`            |
| `ARM_LDR_I`  `op1`  `<-`  `mem`  `[`  `op2`  `+`  `op3`   `]` <br/>`op3` 的范围是 `-`  `4095`  `~`  `+`  `4095` <br/> `LDR`  `op1`  `,`  `[`  `op2`  `,`  `#`  `op3`  `] ` `!` | `struct Register *` | `struct Register *` | `struct Immi_12 *`  | `NULL`            |
| `ARM_LDR_I_PRE`  `op1`  `<-`  `mem`  `[`  `op2`  `+`  `op3`   `]` <br/> `op2`  `<-`  `op2`  `+`  `op3` <br/>`op3` 的范围是 `-`  `4095`  `~`  `+`  `4095` <br/> `LDR`  `op1`  `,`  `[`  `op2`  `,`  `#`  `op3`  `] ` `!` | `struct Register *` | `struct Register *` | `struct Immi_12 *`  | `NULL`            |
| `ARM_LDR_I_POST`  `op1`  `<-`  `mem`  `[`  `op2`  `]` <br/> `op2`  `<-`  `op2`  `+`  `op3` <br/>`op3` 的范围是 `-`  `4095`  `~`  `+`  `4095` <br/> `LDR`  `op1`  `,`  `[`  `op2`  `]`  `,`  `#`  `op3` | `struct Register *` | `struct Register *` | `struct Immi_12 *`  | `NULL`            |
| `ARM_LDR_R`  `op1`  `<-`  `mem`  `[`  `op2`  `+`  `op3`   `]` <br/>`op3` 中的 `shift_op` 只能为立即数 <br/> `LDR`  `op1`  `,`  `[`  `op2`  `,`  `op3`  `] ` | `struct Register *` | `struct Register *` | `struct Operand2 *` | `NULL`            |
| `ARM_LDR_R_PRE`  `op1`  `<-`  `mem`  `[`  `op2`  `+`  `op3`   `]` <br/> `op2`  `<-`  `op2`  `+`  `op3` <br/>`op3` 中的 `shift_op` 只能为立即数 <br/> `LDR`  `op1`  `,`  `[`  `op2`  `,`  `op3`  `] ` | `struct Register *` | `struct Register *` | `struct Operand2 *` | `NULL`            |
| `ARM_LDR_R_POST`  `op1`  `<-`  `mem`  `[`  `op2`  `]` <br/> `op2`  `<-`  `op2`  `+`  `op3` <br/>`op3` 的范围是 `-`  `4095`  `~`  `+`  `4095` <br/> `LDR`  `op1`  `,`  `[`  `op2`  `]`  `,`  `op3` | `struct Register *` | `struct Register *` | `struct Operand2*`  | `NULL`            |
| `ARM_STR_I`  `mem`  `[`  `op2`  `+`  `op3`   `]`  `<-`  `op1`  <br/>`op3` 的范围是 `-`  `4095`  `~`  `+`  `4095` <br/> `STR`  `op1`  `,`  `[`  `op2`  `,`  `#`  `op3`  `] ` `!` | `struct Register *` | `struct Register *` | `struct Immi_12 *`  | `NULL`            |
| `ARM_LDR_I_PRE`  `mem`  `[`  `op2`  `+`  `op3`   `]`   `<-`  `op1` <br/> `op2`  `<-`  `op2`  `+`  `op3` <br/>`op3` 的范围是 `-`  `4095`  `~`  `+`  `4095` <br/> `STR`  `op1`  `,`  `[`  `op2`  `,`  `#`  `op3`  `] ` `!` | `struct Register *` | `struct Register *` | `struct Immi_12 *`  | `NULL`            |
| `ARM_STR_I_POST`  `mem`  `[`  `op2`  `]`   `<-`  `op1` <br/> `op2`  `<-`  `op2`  `+`  `op3` <br/>`op3` 的范围是 `-`  `4095`  `~`  `+`  `4095` <br/> `STR`  `op1`  `,`  `[`  `op2`  `]`  `,`  `#`  `op3` | `struct Register *` | `struct Register *` | `struct Immi_12 *`  | `NULL`            |
| `ARM_STR_R`  `mem`  `[`  `op2`  `+`  `op3`   `]`  `<-`  `op1` <br/>`op3` 中的 `shift_op` 只能为立即数 <br/> `LDR`  `op1`  `,`  `[`  `op2`  `,`  `op3`  `] ` | `struct Register *` | `struct Register *` | `struct Operand2 *` | `NULL`            |
| `ARM_STR_R_PRE`  `mem`  `[`  `op2`  `+`  `op3`   `]`   `<-`  `op1` <br/> `op2`  `<-`  `op2`  `+`  `op3` <br/>`op3` 中的 `shift_op` 只能为立即数 <br/> `STR`  `op1`  `,`  `[`  `op2`  `,`  `op3`  `] ` | `struct Register *` | `struct Register *` | `struct Operand2 *` | `NULL`            |
| `ARM_STR_R_POST`  `mem`  `[`  `op2`  `]`   `<-`  `op1` <br/> `op2`  `<-`  `op2`  `+`  `op3` <br/>`op3` 的范围是 `-`  `4095`  `~`  `+`  `4095` <br/> `STR`  `op1`  `,`  `[`  `op2`  `]`  `,`  `op3` | `struct Register *` | `struct Register *` | `struct Operand2*`  | `NULL`            |

  