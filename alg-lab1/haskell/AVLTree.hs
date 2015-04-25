module AVLTree(AVLTree,emptyAVL) where

data (Ord a,Show a) => AVLTree a = EmptyAVL
								 | NodeAVL a (AVLTree a) (AVLTree a)
	deriving Show

emptyAVL = EmptyAVL

--------------------------------------------------------------------------------
-- height
--------------------------------------------------------------------------------

-- Ett tomt träd har så klart noll i höjd.
height EmptyAVL = 0

-- Om vi bara kan gå åt vänster så ökar vi höjden med ett och fortsätter nedåt
-- rekursivt.
height (NodeAVL v lf EmptyAVL) = 1 + (height lf)

-- Samma sak om vi bara kan gå åt höger.
height (NodeAVL v EmptyAVL rt) = 1 + (height rt)

-- Om vi kan gå åt både vänster och höger så returnerar vi det högsta höjd-
-- värdet av de två underträden.
height (NodeAVL v lf rt) = max (height lf) (height rt)

--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
-- rotateLeft
--------------------------------------------------------------------------------
-- 
-- http://upload.wikimedia.org/wikipedia/commons/2/23/Tree_rotation.png
--
-- Bilden illustrerar rotationen. Vi tar Q, P och A. P ersätts med Q, som får Ps
-- som sitt vänstra underträd. P får Qs vänstra underträd som sitt högra
-- underträd.
rotateLeft (NodeAVL              -- Q i bilden
				q_value
				q_left_tree      -- A
				(NodeAVL         -- P
					p_value
					p_left_tree
					p_right_tree
				)
			)
	= (NodeAVL p_value (NodeAVL q_value q_left_tree p_left_tree) p_right_tree)
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
-- rotateRight
--------------------------------------------------------------------------------
-- http://upload.wikimedia.org/wikipedia/commons/2/23/Tree_rotation.png
--
-- Bilden illustrerar rotationen. Vi tar Q, P och C. Q ersätts med P, och
-- behåller Ps underträd åt vänster, men får Q som sitt högra delträd. Ps
-- högra delträd sätts som vänsterträd under Q.
rotateRight (NodeAVL              -- Q
				q_value
				(NodeAVL          -- P
					p_value
					p_left_tree
					p_right_tree
				)
				q_right_tree      -- C
			)
	= (NodeAVL p_value p_left_tree (NodeAVL q_value p_right_tree q_right_tree))
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
-- rotateRightLeft 
--------------------------------------------------------------------------------
-- Först roterar vi höger (innerst) sen vänster (ytterst).
rotateRightLeft (NodeAVL v lf rt) = rotateLeft (NodeAVL v lf (rotateRight rt))
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
-- rotateLeftRight 
--------------------------------------------------------------------------------
-- Först roterar vi vänster (innerst) sen höger (ytterst).
rotateLeftRight (NodeAVL v lf rt) = rotateRight (NodeAVL v (rotateLeft lf) rt)
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
-- fixLeftHeavy
--------------------------------------------------------------------------------
fixLeftHeavy at@(NodeAVL a bt@(NodeAVL b bl br) ar)
	| bh - arh < 2 = NodeAVL a bt ar
	| height bl < height br = rotateLeftRight at
	| otherwise = rotateRight at
	where 	arh = height ar
		bh = height bt
		ah = height at
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
-- fixRightHeavy
--------------------------------------------------------------------------------
fixRightHeavy at@(NodeAVL a al bt@(NodeAVL b bl br))
	| bh - alh < 2 = NodeAVL a al bt
	| height bl > height br = rotateRightLeft at
	| otherwise = rotateLeft at
	where 	bh = height bt
		alh = height al
		ah = height at
--------------------------------------------------------------------------------
		
treeInsert x EmptyAVL = NodeAVL x EmptyAVL EmptyAVL

treeInsert x (NodeAVL a al ar)
	| x < a     = fixLeftHeavy  (NodeAVL a (treeInsert x al) ar)
	| otherwise = fixRightHeavy (NodeAVL a al (treeInsert x ar))

buildTree lf = foldr treeInsert EmptyAVL (reverse lf)

t1::AVLTree Integer
t1= NodeAVL 3 (NodeAVL 2 (NodeAVL 1 EmptyAVL EmptyAVL) EmptyAVL) EmptyAVL

t2::AVLTree Integer
t2= NodeAVL 1 EmptyAVL (NodeAVL 2 EmptyAVL (NodeAVL 3 EmptyAVL EmptyAVL)) 

t3::AVLTree Integer
t3= NodeAVL 1 EmptyAVL (NodeAVL 3 (NodeAVL 2 EmptyAVL EmptyAVL) EmptyAVL)

t4::AVLTree Integer
t4= NodeAVL 3 (NodeAVL 1 EmptyAVL (NodeAVL 2 EmptyAVL EmptyAVL)) EmptyAVL
