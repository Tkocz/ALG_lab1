--module BinTree (BinTree,emptyTree,treeMember,treeInsert,
--                        treeDelete, buildTree,inorder,treeSort) where

--treeMember   :: (Ord a,Show a) => a -> BinTree a -> Bool
--treeInsert   :: (Ord a,Show a) => a -> BinTree a -> BinTree a
--treeDelete   :: (Ord a,Show a) => a -> BinTree a -> BinTree a
--buildTree    :: (Ord a,Show a) => [a] -> BinTree a
--inorder      :: (Ord a,Show a) => BinTree a -> [a]
--treeSort     :: (Ord a,Show a) => [a] -> [a]
module BinTree () where
data (Ord a) => BinTree a = EmptyBT
                          | NodeBT a (BinTree a) (BinTree a)
    deriving Show

emptyTree = EmptyBT

--------------------------------------------------------------------------------
-- treeMember
--------------------------------------------------------------------------------
-- En tom nod innehåller inget värde, så den kan omöjligt innehålla det värde vi
-- söker. Därför returnerar vi falskt.
treeMember v' EmptyBT = False

-- En nod som inte är tom kan antingen innehålla värdet vi söker, eller så kan
-- dess två delträd (vänster- och högerträden), varvid vi genomsöker dem
-- rekursivt.
treeMember v' (NodeBT v lf rt)

	-- Noden vi tittar på just nu innehåller värdet vi söker. Vi har alltså
	-- hittat det och returnerar därför sant.
	| v == v'
	= True

	-- Om v' är mindre än prim så finns noden (om den existerar) i vänster
	-- delträd. Därför anropar vi treeMember rekursivt på det vänstra delträdet.
	| v' < v
	= treeMember v' lf
	
	-- Om v' är större än prim så finns noden (om den existerar) i höger
	-- delträd. Därför anropar vi treeMember rekursivt på det högra delträdet.
    | v' > v
	= treeMember v' rt
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
-- treeInsert
--------------------------------------------------------------------------------
-- Försöker vi sätta in ett värde i en tom nod så ersätter vi helt enkelt den
-- tomma noden med en nod som innehåller det värde vi vill, samt två tomma
-- barnnoder.
treeInsert v' EmptyBT = NodeBT v' EmptyBT EmptyBT

treeInsert v' (NodeBT v lf rt)
	-- Vi har hittat en nod som innehåller det värde vi vill sätta in i sökträdet.
	-- Då ett sökträd inte får innehålla samma värde i flera noder så anser vi
	-- helt enkelt att vi är klara här, och returnerar en nod som är exakt
	-- likadan som den som redan finns i sökträdet.
	| v'==v
	= NodeBT v lf rt
	
	-- Den nod vi tittar på just nu har ett större värde än v', vilket innebär att
	-- värdet vi vill sätta in ska sättas in i nuvarande nods vänster delträd, så
	-- vi skapar en nod likt den vi redan har, fast anropar treeInsert på vänster
	-- delträd.
    | v'<v
	= NodeBT v (treeInsert v' lf) rt
	
	-- Den nod vi tittar på just nu har ett mindre värde än v', vilket innebär att
	-- värdet vi vill sätta in ska sättas in i nuvarande nods höger delträd, så
	-- vi skapar en nod likt den vi redan har, fast anropar treeInsert på höger
	-- delträd.
    | otherwise --(v'>v)
	= NodeBT v lf (treeInsert v' rt)
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
-- buildTree
--------------------------------------------------------------------------------
-- Jag har ingen aning om hur den här funktionen fungerar, någon kan ju försöka
-- lista ut det. Jag tror att den anropar treeInsert på alla element i listan
-- lf, rekursivt på något vis som ingen fattar.
buildTree lf = foldr treeInsert EmptyBT (reverse lf)
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
-- treeDelete
--------------------------------------------------------------------------------
-- Värdet vi söker finns inte i sökträdet, så det finns inget att ta bort.
treeDelete v' EmptyBT = EmptyBT

-- Vi har hittat en nod med ett värde, men noden har inga barnnoder.
treeDelete v' (NodeBT v EmptyBT EmptyBT)
	-- Det värde vi vill ta bort finns i den nod vi tittar på just nu, så vi tar
	-- bort den här noden.
	| v'==v
	= EmptyBT

	-- Vi har inte hittat värdet i den här noden, så vi återställer noden så den
	-- får vara precis som den är. Ingen ändring görs i trädet.
	| otherwise
	= (NodeBT v EmptyBT EmptyBT)
             
-- Vi har hittat en nod som har ett delträd åt vänster, och dessutom innehåller
-- det värde vi vill ta bort. Vi löser det genom att ta bort nuvarande nod och
-- ersätta den med sitt delträd:
--
--       6
--      / \
--     5   7
--    /
--   3
--  / \
-- 2   4
--
-- Vi tar bort 5:an genom att ersätta den noden med sitt vänstra delträd:
--
--       6
--      / \
--     3   7
--    / \
--   2   4
--
treeDelete v' (NodeBT v lf EmptyBT) | v'==v = lf

-- Här gör vi samma sak som ovan, fast för höger delträd.
treeDelete v' (NodeBT v EmptyBT rt) | v'==v = rt

-- När en nod har två barnnoder (både vänster- och högerdelträd) blir det lite
-- klurigare. Vi gör som labbspecen föreslår:
--   Det svåraste fallet är då noden som skall tas bort har två barn, och då kan
--   man faktiskt lösa det hela på lite olika sätt – kravet är ju att det
--   resulterande trädet fortfarande skall vara ett binärt sökträd. Vi väljer
--   där följande strategi: Den nod A som skall tas bort ersätts med den nod B i
--   A:s högra underträd som har ett minimalt värde. Därefter tas B bort (med
--   treeDelete förstås). 
treeDelete v' (NodeBT v lf rt)
	-- Nuvarande nod innehåller värdet vi söker, så den ska bort. Vi tillämpar
	-- strategin som labbspecen erbjuder:
	--
	-- (minTree rt)
	--   Nuvarande nod får det värde som är det lägsta värdet i höger underträd.
	--
	-- lf
	--   Vänster underträd lämnar vi orört.
	--
	-- (treeDelete (minTree rt) rt))
	--  Vi återanvänder höger underträd, fast UTAN sitt lägsta värde.
	--
	| v' == v
	= (NodeBT (minTree rt) lf (treeDelete (minTree rt) rt))
	
	-- Det värde vi vill ta bort är mindre än nuvarande nods värde, så vi letar
	-- vidare nedåt vänster i trädet rekursivt.
	| v' < v
	= (NodeBT v (treeDelete v' lf) rt)
	
	-- Samma som ovan, fast åt höger istället då v' är större än v.
	| v' > v
	= (NodeBT v lf (treeDelete v' rt))
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
-- minTree
--------------------------------------------------------------------------------
-- Inga mer barnnoder finns, så detta måste vara lägsta värdet.
minTree (NodeBT v EmptyBT EmptyBT) = v

-- Alla lägre värden ligger åt vänster i ett binärt sökträd, så vi jobbar åt
-- vänster...
minTree (NodeBT v lf EmptyBT) = minTree lf

-- ...förutom i noder där vi bara kan gå åt höger. Där går vi åt höger istället.
minTree (NodeBT v EmptyBT rt) = minTree rt

-- Om vi kan välja håll så går vi åt vänster, eftersom lägre värden ligger ditåt.
minTree (NodeBT v lf rt) = minTree lf
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
-- inorder 
--------------------------------------------------------------------------------
-- Om vi når en nod utan underträd så returnerar vi nodens värde som en mängd.
inorder (NodeBT v EmptyBT EmptyBT) = [v]

-- Om vi har ett underträd åt vänster så går vi igenom vänster underträd rekursivt
-- och lägger sedan till nuvarande nods värde i slutet på mängden.
inorder (NodeBT v lf EmptyBT) = (inorder lf)++[v]

-- Vi gör samma sak åt höger, fast lägger på nuvarande nods värde i början på mängden
-- istället.
inorder (NodeBT v EmptyBT rt) = [v]++(inorder rt)

-- Har vi underträd åt båda hållen går vi först igenom vänster underträd rekursivt,
-- sätter sedan in nuvarande nods värde i mängden, och lägger sedan på höger underträds
-- värden rekursivt.
inorder (NodeBT v lf rt) = (inorder lf)++[v]++(inorder rt)
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
-- treeSort
--------------------------------------------------------------------------------
-- Alltså, vi skickar bara vidare allt till buildTree och plockar ut det med
-- inorder...
treeSort x = inorder (buildTree x)
--------------------------------------------------------------------------------

t1:: BinTree Integer
t1= NodeBT 5 (NodeBT 2 EmptyBT EmptyBT) EmptyBT

t2:: BinTree Integer
t2= NodeBT 5 (NodeBT 2 EmptyBT (NodeBT 4 (NodeBT 3 EmptyBT EmptyBT) EmptyBT)) EmptyBT

t3::BinTree Integer
t3= NodeBT 5 (NodeBT 2 EmptyBT (NodeBT 4 (NodeBT 3 EmptyBT EmptyBT) EmptyBT)) (NodeBT 6 EmptyBT (NodeBT 7 EmptyBT (NodeBT 10 (NodeBT 9 EmptyBT EmptyBT) (NodeBT 11 EmptyBT EmptyBT))))



