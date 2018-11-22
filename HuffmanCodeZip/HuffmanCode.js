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
    node.m_weight = 0;
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
    codeTerm.m_times = 0;
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
    tree.m_sum = 0;
    tree.m_isUse = false;
    //#endregion

    var _t = tree;
    //#region func
    tree.DelNode = () => {};
    tree.SetBitSet = (codingTable) => {
        var s = "";
        if (_t.m_root.m_leftChild != null) {
            SetBitSetRecursion(codingTable, s + "0", _t.m_root.m_leftChild);
        }
        if (_t.m_root.m_rightChild != null) {
            SetBitSetRecursion(codingTable, s + "1", _t.m_root.m_rightChild);
        }
    };
    tree.SetRootChar = () => {};
    tree.BitsetToChar = () => {};
    tree.Add = (t2) => {
        var t = CreateTree();
        t.m_root = CreateNode();
        t.m_root.m_leftChild = _t.m_root;
        t.m_root.m_rightChild = t2.m_root;
        t.m_root.m_weight = _t.m_root.m_weight + t2.m_root.m_weight;
        t.m_sum = t.m_root.m_weight;
        return t;
    }

    /**
     * 
     * @param {Array} codingTable 
     * @param {string} s 
     * @param {Node} n 
     */
    function SetBitSetRecursion(codingTable, s, n) {
        if (n.m_leftChild == null && n.m_rightChild == null) {
            for (var i = 0; i < codingTable.length; i++) {
                if (n.m_char == codingTable[i].m_char) {
                    codingTable[i].m_bitset = s;
                    return;
                }
            }
        }

        if (n.m_leftChild != null) {
            SetBitSetRecursion(codingTable, s + "0", n.m_leftChild);
        }

        if (n.m_rightChild != null) {
            SetBitSetRecursion(codingTable, s + "1", n.m_rightChild);
        }
    }
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
    huffman.m_hashTable = null;
    huffman.m_compressedStr = null;
    huffman.m_deCompressedStr = null;
    //#endregion
    var _h = huffman;
    //#region func
    huffman.SetCompressCodingTable = () => {
        _h.m_codingTable = [];
        _h.m_treeset = [];

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

        _h.SetTree();
        _h.m_tree.SetBitSet(_h.m_codingTable);
        _h.SetHash();
    };

    huffman.SetDeCompressCodingTable = () => {

        _h.m_codingTable = [];
        _h.m_treeset = [];

        var size = ConvertCharsToDec(_h.m_deCompressStr[0]);

        var offset = 0 + 1; // size

        for (var i = 0; i < size; i++) {
            var codeTerm = CreateCodeTerm();
            codeTerm.m_char = _h.m_deCompressStr[offset];
            offset++;
            codeTerm.m_times = ConvertCharsToBinString(_h.m_deCompressStr.substr(offset, 4));
            offset += 4;
            _h.m_codingTable.push(codeTerm)
        }
        //make tree set
        _h.SetTree();
        _h.m_tree.SetBitSet(_h.m_codingTable);
        _h.SetHash();
    }

    huffman.SetTree = () => {
        //make tree set
        for (var i = 0, leni = _h.m_codingTable.length; i < leni; i++) {
            var tree = CreateTree();
            tree.m_root = CreateNode();
            tree.m_root.m_char = _h.m_codingTable[i].m_char
            tree.m_root.m_weight = _h.m_codingTable[i].m_times;
            tree.m_sum = _h.m_codingTable[i].m_times;
            tree.m_isUse = false;
            _h.m_treeset.push(tree);
        }

        //make huffman tree
        if (_h.m_treeset.length <= 1) {
            //You should not compress a empty file(or with only a word)!
        } else {
            for (var i = 0, leni = _h.m_treeset.length - 1; i < leni; i++) {
                //find 2 tree which is the smallest
                var minw = Infinity;
                var index1 = -1;
                for (var j = 0, lenj = _h.m_treeset.length; j < lenj; j++) {
                    if (_h.m_treeset[j].m_isUse) {
                        continue;
                    }
                    if (minw > _h.m_treeset[j].m_sum) {
                        minw = _h.m_treeset[j].m_sum;
                        index1 = j;
                    }
                }
                _h.m_treeset[index1].m_isUse = true;

                minw = Infinity;
                var index2 = -1;
                for (var j = 0, lenj = _h.m_treeset.length; j < lenj; j++) {
                    if (_h.m_treeset[j].m_isUse) {
                        continue;
                    }
                    if (minw > _h.m_treeset[j].m_sum) {
                        minw = _h.m_treeset[j].m_sum;
                        index2 = j;
                    }
                }
                _h.m_treeset[index2].m_isUse = true;

                //why when index2 < index1 it doesn`t change the pos
                if (index1 <= index2) {
                    _h.m_treeset[index1] = _h.m_treeset[index1].Add(_h.m_treeset[index2]);
                    _h.m_treeset[index1].m_isUse = false;
                } else {
                    _h.m_treeset[index2] = _h.m_treeset[index1].Add(_h.m_treeset[index2]);
                    _h.m_treeset[index2].m_isUse = false;
                }
            }
        }

        _h.m_tree = _h.m_treeset[0];
    }

    huffman.SetHash = () => {
        _h.m_hashTable = new HashTable();
        for (var i = 0, len = _h.m_codingTable.length; i < len; i++) {
            _h.m_hashTable.add(_h.m_codingTable[i].m_char, _h.m_codingTable[i].m_bitset);
        }
    };

    huffman.Compress = () => {
        var content = "";
        _h.SetCompressCodingTable();
        //write the coding table length : 1 Byte
        var cl = ConvertDecToChar(_h.m_codingTable.length);
        if (cl.length != 1) {
            alert("Something wrong with the bin transform in CreateHuffman: cl " + cl);
        }
        content += cl;
        //write the coding table
        for (var i = 0, len = _h.m_codingTable.length; i < len; i++) {
            content += _h.m_codingTable[i].m_char;
            content += ConvertDecTo4Char(_h.m_codingTable[i].m_times);
        }

        //write the compress content

        //get bin str
        var binStr = "";
        for (var i = 0, len = _h.m_compressStr.length; i < len; i++) {
            binStr += _h.m_hashTable.getValue(_h.m_compressStr[i]);
        }

        content += ConvertBinToChars(binStr);

        _h.m_compressedStr = content;
    };

    huffman.DeCompress = () => {
        //read coding table
        _h.SetDeCompressCodingTable();
    };

    //#endregion
    return huffman;
}

//var HuffmanUtil = CreateHuffman();