/**
 *  Neum
 *  
 *  2018 11 13
 * 
 *  Huffman Code to compress and decompress js code
 * 
 */

//Tree Node
function CreateNode() {
    var node = new Object();

    //#region property
    node.m_char = null;
    node.m_weight = null;
    node.m_leftChild = null;
    node.m_rightChild = null;
    //#endregion

    return node;
}

//CodeTerm
function CreateCodeTerm() {
    var codeTerm = new Object();

    //#region property
    codeTerm.m_char = null;
    codeTerm.m_times = null;
    codeTerm.m_bitset = null;
    //#endregion

    //#region func
    //#endregion


    return codeTerm;
}

//Tree
function CreateTree() {
    var tree = new Object();

    //#region property
    tree.m_root = null;
    tree.m_sum = null;
    tree.m_isUse = null;
    //#endregion

    //#region func
    tree.DelNode = () => {};
    tree.SetBitSet = () => {};
    tree.SetRootChar = () => {};
    tree.BitsetToChar = () => {};

    //#endregion

    return tree;
}

//Huffman Tree
function CreateHuffman() {
    var huffman = new Object();
    //#region property
    huffman.m_codingTable = null;
    huffman.m_treeset = null;
    huffman.m_tree = null;
    huffman.m_compressStr = null;
    huffman.m_deCompressStr = null;
    //#endregion
    var _h = huffman;
    //#region func
    huffman.SetCodingTable = () => {
        _h.m_codingTable = [];

        //record char and times
        for (var i = 0, leni = _h.m_compressStr.length; i < leni; i++) {
            var j = 0;
            for (lenj = _h.m_codingTable.length; j < lenj; j++) {
                if (_h.m_codingTable[j].m_char == _h.m_compressStr[i]) {
                    _h.m_codingTable[j].m_times++;
                    break;
                }
            }
            if (j == _h.m_codingTable.length) {
                var codeTerm = CreateCodeTerm();
                codeTerm.m_char = _h.m_compressStr[i];
                codeTerm.m_times = 1;
                _h.m_codingTable.push(codeTerm);
            }
        }

        //make tree set
        for (var i = 0, leni = _h.m_codingTable.length; i < leni; i++) {
            var tree = CreateTree();
            tree.m_root = CreateNode();
            tree.m_root.m_char = _h.m_codingTable[i].m_char
            tree.m_root.m_weight = _h.m_codingTable[i].m_times;
            tree.m_isUse = false;
            _h.m_treeset.push(tree);
        }

        //make huffman tree
        if (_h.m_treeset.length <= 1) {
            //You should not compress a empty file(or with only a word)!
        } else {
            for (var i = 0, leni = _h.m_treeset.length - 1; i < leni; i++) {
                //find 2 tree which is the smallest
                var minw = Math.max;
                var index1 = -1;
                for (var j = 0, lenj = _h.m_treeset.length; j < lenj; j++) {
                    if (_h.m_treeset[i].m_isUse) {
                        continue;
                    }
                    if (minw > _h.m_treeset[j].m_sum) {
                        minw = _h.m_treeset[j].m_sum;
                        index1 = k;
                    }
                }
                _h.m_treeset[index1].m_isUse = true;

                minw = Math.max;
                var index2 = -1;
                for (var j = 0, lenj = _h.m_treeset.length; j < lenj; j++) {
                    if (_h.m_treeset[i].m_isUse) {
                        continue;
                    }
                    if (minw > _h.m_treeset[j].m_sum) {
                        minw = _h.m_treeset[j].m_sum;
                        index2 = k;
                    }
                }
                _h.m_treeset[index2].m_isUse = true;

                if (index1 < index2) {
                    
                }
            }
        }
    };

    huffman.Compress = () => {
        _h.SetCodingTable();
    };

    huffman.DeCompress = () => {};

    //#endregion
    return huffman;
}

var HuffmanUtil = CreateHuffman();