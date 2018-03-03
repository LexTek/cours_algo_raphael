let rec plus_one_list lst =
  match lst with
  | []     -> []
  | hd::tl -> (hd + 1) :: (plus_one_list tl)
in

let lst = [1; 2; 3] in
let plus_lst = plus_one_list lst in
(lst, plus_lst)

type 'a bin_tree =
  | Leaf
  | Node of 'a * 'a bin_tree * 'a bin_tree


(*************** SEARCH BIN TREE *****************)

type s_bin_tree = int bin_tree

let rec insert_elem sbtree e =
  match sbtree with
  | Leaf                  -> Node (e, Leaf, Leaf)
  | Node (v, left, right) ->
    if (e < v) then
      Node (v, insert_elem left e, right)
    else
      Node (v, left, insert_elem right e)
;;

let rec list_to_bin_tree lst btree =
  match lst with
  | []       -> btree
  | hd :: tl -> list_to_bin_tree tl (insert_elem btree hd)
;;

let list_test = [5;2;6;0;1;7;8] in
list_to_bin_tree list_test Leaf 

let list_to_bin_tree_fold lst btree = List.fold_left (fun bt e -> insert_elem bt e) btree lst
