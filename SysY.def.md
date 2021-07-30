`VN` = { `CompUnit`, `Decl`, `ConstDecl`, `ValDecl`, `BType`, `ConstDef`, `ConstDefs`, `ConstInitVal`, `ConstInitVals`, `ArrayDefs`, `ArrayDef`, `VarDecl`, `VarDef`, `VarDefs`, `InitVal`, `InitVals`, `FuncDef`, `FuncType`, `FuncFParams`, `FnucFParam`, `FArrayDefs`, `FArrayDef`, `Block`, `BlockItem`, `BlockItems`, `Stmt`, `Exp`, `Cond`, `LVal`, `PrimaryExp`, `Number`, `UnaryExp`, `UnaryOp`, `FuncRParams`, `MulExp`, `AddExp`, `RelExp`, `EqExp`, `LAndExp`, `LOrExp`, `ConstExp`, `Ident`, `Digit`, `IdentNonDigit`, `IntConst`,  `DecConst`, `NonZeroDigit`, `OctConst`, `OctDigit`, `HexConst`, `HexDigit`, `Comment`, `CommentSL`, `CommentML`, `CommentMLContent`, `CommentMLContents`, `Printable`, `Printables`, `String`  }

`VT` = { `!`, `"`, `#`, `$`, `%`, `&`, `'`, `(`, `)`, `*`, `+`, `,`, `-`, `.`, `/`, `0`, `1`,`2`,`3`,`4`,`5`,`6`,`7`,`8`,`9`,`:`,`;`,`<`,`=`,`>`,`?`,`@`,`A`,`B`,`C`,`D`,`E`,`F`,`G`,`H`,`I`,`J`,`K`,`L`,`M`,`N`,`O`,`P`,`Q`,`R`,`S`,`T`,`U`,`V`,`W`,`X`,`Y`,`Z`,`[`,`\`,`]`,`^`,`_`,`a`,`b`,`c`,`d`,`e`,`f`,`g`,`h`,`i`,`j`,`k`,`l`,`m`,`n`,`o`,`p`,`q`,`r`,`s`,`t`,`u`,`v`,`w`,`x`,`y`,`z`,`{`,`|`,`}`, ` `,`\n`, `\r`, `\t` }

`G` [ `CompUnit` ]  :

​		`CompUnit`   -> `CompUnit`  `Decl` | `CompUnit` `FuncDef` | `Decl` | `FuncDef`

​		`Ident`		 -> `IdentNonDigit` 

​								| `Ident`  `IdentNonDigit`

​								| `Ident` `Digit`

​		`Digit`		 -> `0` | `1` | `2` | `3` | `4` | `5` | `6` | `7` | `8` | `9`

​		`IdentNonDigit`	-> `A` | `B` | `C` | `D` | `E` | `F` | `G` | `H` | `I` | `J` | `K` | `L` | `M` | `N` | `O` | `P` | `Q` | `R` | `S` | `T` | `U` | `V` | `W` | `X` | `Y` | `Z` | `a` | `b` | `c` | `d` | `e` | `f` | `g` | `h` | `i` | `j` | `k` | `l` | `m` | `n` | `o` | `p` | `q` | `r` | `s` | `t` | `u` | `v` | `w` | `x` | `y` | `z` | `_`

​		`IntConst`	-> `DecConst` | `OctConst` | `HexConst`

​		`DecConst`	-> `NonZeroDigit` | `DecConst` `Digit`

​		`NonZeroDigit`	->   `1` | `2` | `3` | `4` | `5` | `6` | `7` | `8` | `9`

​		`OctConst`	-> `0` | `OctConst` `OctDigit`

​		`OctDigit`	->  `0` | `1` | `2` | `3` | `4` | `5` | `6` | `7` 

​		`HexConst`	-> `0` `x` `HexDigit` | `0` `X` `HexDigit` | `HexConst` `HexDigit`

​		`HexDigit`	-> `0` | `1` | `2` | `3` | `4` | `5` | `6` | `7` | `8` | `9` | `A` | `B` | `C` | `D` | `E` | `F` | `a` | `b` | `c` | `d` | `e` | `f`

​		`Printable`	->  `!` | `"` | `#` | `$` | `%` | `&` | `'` | `(` | `)` | `*` | `+` | ` |` | `-` | `.` | `/` | `0` | `1` | `2` | `3` | `4` | `5` | `6` | `7` | `8` | `9` | `:` | `;` | `<` | `=` | `>` | `?` | `@` | `A` | `B` | `C` | `D` | `E` | `F` | `G` | `H` | `I` | `J` | `K` | `L` | `M` | `N` | `O` | `P` | `Q` | `R` | `S` | `T` | `U` | `V` | `W` | `X` | `Y` | `Z` | `[` | `\` | `]` | `^` | `_` | `a` | `b` | `c` | `d` | `e` | `f` | `g` | `h` | `i` | `j` | `k` | `l` | `m` | `n` | `o` | `p` | `q` | `r` | `s` | `t` | `u` | `v` | `w` | `x` | `y` | `z` | `{` | `|` | `}` |  `  `

​		`Printables`	->  `Printable`

​										| `Printables` `Printable`

​		`String`	-> `"` `Printables` `"`

​		`Comment`	-> `CommentSL` | `CommentML`

​		`CommentSL`	-> `/` `/` `Printables`

​		`CommentML`	-> `/ ` `*` `CommentMLContents` `*` `/`

​		`CommentMLContent`	-> `Printable` | `\t` | `\n` | `\r` 

​		`CommentMLContents`	-> `CommentMLContent`

​													| `CommentMLContent` `CommentMLContents`

​		`Decl`			-> `ConstDecl` | `ValDecl`

​		`ConstDecl`	-> `c` `o` `n` `s` `t` `BType` `ConstDefs` `;`

​		`BType`	-> `i` `n` `t` 

​		`ConstDefs`	-> `ConstDef` | `ConstDefs` `,` `ConstDef`

​		`ConstDef`	-> `Ident` `=` `ConstInitVal`

​									| `Ident` `ArrayDefs` `=` `ConstInitVal`

​		`ArrayDefs`	-> `ArrayDef` | `ArrayDefs` `ArrayDef`

​		`ArrayDef`	 -> `[` `ConstExp` `]`

​		`ConstInitVal`	-> `ConstExp` | `{` `}` | `{` `ConstInitVals` `}`

​		`ConstInitVals`	-> `ConstInitVal` | `ConstInitVals` `,` `ConstInitVal`

​		`VarDecl`	-> `BType` `VarDefs` `;`

​		`VarDefs`	-> `VarDef` | `VarDefs` `,` `ValDef`

​		`VarDef`	-> `Ident` | `Ident` `ArrayDefs` 

​							| `Ident` `=` `InitVal` | `Ident` `ArrayDefs` `=` `InitVal`

​		`InitVal`	-> `Exp` | `{  ` `}` | `{` `InitVals` `}`

​		`InitVals`	-> `InitVal` | `InitVals` `,` `InitVal`

​		`FuncDef`	-> `FuncType` `Ident` `(` `)` `Block` 

​								| `FuncType` `Ident` `(` `FuncFParams` `)` `Block` 

​		`FuncType`	-> `v` `o` `i` `d` | `i` `n` `t`

​		`FuncFParams`	-> `FuncFParam` | `FuncFParams` `,` `FuncFParam`

​		`FuncFParam`	-> `BType` `Ident` | `BType` `Ident` `[` `]` | `BType` `Ident` `[` `]` `FArrayDefs`

​		`FArrayDefs`	-> `FArrayDef` | `FArrayDefs` `FArrayDef`

​		`FArrayDef`	-> `[` `Exp` `]`

​		`Block`	-> `{` `}` | `{` `BlockItems` `}`

​		`BlockItems`	-> `BlockItem` | `BlockItems` `BlockItem`

​		`BlockItem`	-> `Decl` | `Stmt`

​		`Stmt`	-> `LVal` `=` `EXP` `;` | `;` | `EXP` `;` | `Block` 

​						| `i` `f` `(` `Cond` `)` `Stmt` | `i` `f` `(` `Cond` `)` `Stmt` `e` `l` `s` `e` `Stmt`

​						| `w` `h` `i` `l` `e` `(` `Cond` `)` `Stmt`

​						| `b` `r` `e` `a` `k` `;` | `c` `o` `n` `t` `i` `n` `u` `e` `;`

​						| `r` `e` `t` `u` `r` `n` `;` | `r` `e` `t` `u` `r` `n` `Exp` `;`

​		`Exp`	->  `AddExp`

​		`Cond`	-> `LOrExp`

​		`LVal`	-> `Ident` | `Ident` `FAarrayDefs` 

​		`PrimaryExp`	-> `(` `Exp` `)` | `LVal` | `Number`

​		`Number`	-> `IntConst`

​		`UnaryExp`	-> `PrimaryExp` | `Ident` `(` `)` | `Ident` `(` `FuncRParams` `)` 

​								| `UnaryOp` `UnaryExp`

​		`UnaryOp`	-> `+` | `-` | `!`

​		`FuncRParams`	-> `Exp` | `FuncRParams` `,` `Exp`

​		`MulExp`	-> `UnaryExp` 

​							| `MulExp` `*` `UnaryExp` 

​							| `MulExp` `/` `UnaryExp` 

​							| `MulExp` `%` `UnaryExp`

​		`AddExp`	-> `MulExp` | `AddExp` `+` `MulExp` |  `AddExp` `-` `MulExp`

​		`RelExp`	-> `AddExp` 

​							| `RelExp` `<` `AddExp`

​							| `RelExp` `>` `AddExp`

​							| `RelExp` `<=` `AddExp`

​							| `RelExp` `>=` `AddExp`

​		`EqExp`	-> `RelExp` | `EqExp` `=` `=` `RelExp` |  `EqExp` `!` `=` `RelExp` 

​		`LAndExp`	-> `EqExp` | `LAndExp` `&` `&` `EqExp`

​		`LOrExp`	-> `LAndExp` | `LOrExp` `|` `|` `LAndExp`

​		`ConstExp` -> `AddExp`

​		