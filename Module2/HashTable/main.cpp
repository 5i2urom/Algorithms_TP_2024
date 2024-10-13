// Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с открытой адресацией.
// Хранимые строки непустые и состоят из строчных латинских букв.
// 1_2. Для разрешения коллизий используйте двойное хеширование.

#include <vector>
#include <string>
#include <iostream>

const size_t DEFAULT_SIZE = 8;

template<class T, class H1, class H2>
class HashTable {
public:
	HashTable( const H1& hasher1, const H2& hasher2, int initialSize = DEFAULT_SIZE );
	~HashTable() {}

    HashTable( const HashTable& other ) :
        hasher1( other.hasher1 ),
        hasher2( other.hasher2 ),
        table( other.table ),
        keysCount( other.keysCount ),
        emptyCount( other.emptyCount ) 
    {}

    HashTable& operator=( const HashTable& other ) {
        hasher1 = other.hasher1;
        hasher2 = other.hasher2;
        table = other.table;
        keysCount = other.keysCount;
        emptyCount = other.emptyCount;

        return *this;
    }

	bool Has( const T& key );
	bool Add( const T& key );
	bool Delete( const T& key );

private:
	H1 hasher1;
    H2 hasher2;

	enum CellState { Empty, Key, Deleted };

	struct HashTableCell {
		T Key;
		CellState State;
        unsigned int Hash1;
        unsigned int Hash2;
		HashTableCell() : Hash1( 0 ), Hash2( 0 ), State( Empty ) {}
        HashTableCell( const T& key, unsigned int hash1, unsigned int hash2, CellState state ) :
            Key( key ), State( state ), Hash1( hash1 ), Hash2( hash2 ) { }
	};
	std::vector<HashTableCell> table;

	unsigned int keysCount;
    unsigned int emptyCount; 

	void RehashTable();
};

template<class T, class H1, class H2>
HashTable<T, H1, H2>::HashTable( const H1& _hasher1, const H2& _hasher2, int initialSize ) :
	hasher1( _hasher1 ),
    hasher2( _hasher2 ),
	table( initialSize, HashTableCell() ),
	keysCount( 0 ),
    emptyCount( initialSize )
{}

template<class T, class H1, class H2>
bool HashTable<T, H1, H2>::Has( const T& key ) {
    for ( int i = 0; i < table.size(); ++i ) {
        unsigned int hash = ( hasher1( key ) + i * hasher2( key ) ) % table.size();
        HashTableCell &cell = table[hash];
        if ( cell.State == Key && cell.Key == key )
            return true;
        else if ( cell.State == Empty )
            return false;        
    }

    return false;
}

template<class T, class H1, class H2>
bool HashTable<T, H1, H2>::Add( const T& key ) {
    if( 4 * emptyCount < table.size() ) // сделать рехеширование, если пустых ячеек < 1/4 размера буфера
        RehashTable();

    unsigned int hash1 = hasher1( key );
    unsigned int hash2 = hasher2( key );
    int firstDeleted = -1;

    for ( int i = 0; i < table.size(); ++i ) {
        unsigned int hash = ( hash1 + i * hash2 ) % table.size();
        HashTableCell &cell = table[hash];
        if ( cell.State == Key && cell.Key == key )
            return false;
        else if ( cell.State == Deleted && firstDeleted < 0)
            firstDeleted = hash;    
        else if ( cell.State == Empty ) {
            int insertIdx = hash;
            if ( firstDeleted >= 0 )
                insertIdx = firstDeleted;
            else
                --emptyCount;
            table[insertIdx] = HashTableCell( key, hash1, hash2, CellState::Key );
            ++keysCount;
            return true;
        }   
    }

    return false;
}

template<class T, class H1, class H2>
bool HashTable<T, H1, H2>::Delete( const T& key ) {
    for ( int i = 0; i < table.size(); ++i ) {
        unsigned int hash = ( hasher1( key ) + i * hasher2( key ) ) % table.size();
        HashTableCell &cell = table[hash];
        if ( cell.State == Key && cell.Key == key ) {
            cell.State = Deleted;
            --keysCount;
            return true;
        }
            
        else if ( cell.State == Empty )
            return false;       
    }

    return false;
}

template<class T, class H1, class H2>
void HashTable<T, H1, H2>::RehashTable() {
    int grow_coeff = ( 3 * keysCount < table.size() ) ? 1 : 2;  // не увеличивать буфер, если ключей < 1/3 размера буфера
	std::vector<HashTableCell> newTable( table.size() * grow_coeff, HashTableCell() );
    int count = 0;
	for( const auto& cell: table ) {
        if ( cell.State == CellState::Key ) {
            for ( int i = 0; i < newTable.size(); ++i ) {
                int newHash = ( cell.Hash1 + i * cell.Hash2 ) % newTable.size();
                if ( newTable[newHash].State == CellState::Empty ) {
                    newTable[newHash] = HashTableCell( cell.Key, cell.Hash1, cell.Hash2, CellState::Key );                    
                    break;
                }
            } 
            ++count;  
        }     
	}

	table = std::move( newTable ); 

    keysCount = count;
    emptyCount = table.size() - keysCount;
}

struct StringHasher {
	unsigned int operator()( const std::string& key ) const {
		unsigned int hash = 0;
		for( unsigned int i = 0; i < key.size(); ++i ) {
			hash = hash * 137 + key[i];
		}
        if ( hash % 2 == 0 )
            hash += 1;
		return hash;
	}
};

struct SimpleStringHasher {
    unsigned int operator()( const std::string& key ) const {
        unsigned int hash = 0;
        for (char ch : key) {
            hash += ch;
        }
        if ( hash % 2 == 0 )
            hash += 1;
        return hash;
	}
};

int main()
{
	StringHasher hasher;
    SimpleStringHasher simpleHasher;
	HashTable<std::string, StringHasher, SimpleStringHasher> table( hasher, simpleHasher );
	char operation = 0;
	std::string word;
	while( std::cin >> operation >> word ) {
		switch( operation ) {
			case '+':
				std::cout << ( table.Add( word ) ? "OK" : "FAIL" ) << std::endl;
				break;
			case '-':
				std::cout << (table.Delete( word ) ? "OK" : "FAIL") << std::endl;
				break;
			case '?':
				std::cout << (table.Has( word ) ? "OK" : "FAIL") << std::endl;
				break;
		}
	}

	return 0;
}
