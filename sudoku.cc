#include<algorithm>
#include<array>
#include<bitset>
#include<exception>
#include<iomanip>
#include<iostream>
#include<fstream>
#include<tuple>
#include<vector>

inline uint countbits(const uint n) {
    return __builtin_popcount(n);
}

class Sudoku {
    private:
		std::vector<std::tuple<uint,uint>> undostack ;
        long numundo = 0 ;
        bool validation = false ;
		std::array<uint,9> row{};
		std::array<uint,9> col{};
		std::array<uint,9> blk{};
		std::array<std::array<uint,9>,9> sudoku{} ;
		std::array<std::array<uint,9>,9> sudoku_test{} ;

        void iset(int r, int c, int v) {
            if (r < 0 || r > 8) {
                throw std::out_of_range("invalid entry, rows must be between 1 and 9.");
            }
            if (c < 0 || c > 8) {
                throw std::out_of_range("invalid entry, columns must be between 1 and 9.");
            }
            if (v < 1 || v > 9) {
                throw std::out_of_range("invalid entry, values must be between 1 and 9.");
            }
            const uint vv = 1<<v ;
            sudoku[r][c] = v ;
            row[r] |= vv ;
            col[c] |= vv ;
            blk[3*int(r/3)+int(c/3)] |= vv ;
            undostack.push_back({r,c});
        }

        void tset(int r, int c, int v) {
            if (r < 0 || r > 8) {
                throw std::out_of_range("invalid entry, row number must be between 1 and 9.");
            }
            if (c < 0 || c > 8) {
                throw std::out_of_range("invalid entry, row column number must be between 1 and 9.");
            }
            if (v < 1 || v > 9) {
                throw std::out_of_range("invalid entry, values must be between 1 and 9.");
            }
            sudoku_test[r][c] = v ;
        }

        void undo() {
            const auto [r,c] = undostack.back() ;
            const auto v = ~(1<<sudoku[r][c]) ;
            sudoku[r][c] = 0 ;
            row[r] &= v ;
            col[c] &= v ;
            blk[3*int(r/3)+int(c/3)] &= v ;
            undostack.pop_back();
            ++numundo ;
        }

        void clear() {
            undostack.clear();
			row.fill(0);
			col.fill(0);
			blk.fill(0);
            for(uint n = 0 ; n < 9 ; ++n) {
                sudoku[n].fill(0) ;
                sudoku_test[n].fill(0) ;
            }
            numundo = 0;
            validation = false;
        }

    public:
        Sudoku() {}
        virtual ~Sudoku() {}

        void init(std::istream& ins) {
            clear();
            ins >> std::skipws ;
            for(uint r = 1; r < 10; ++r) {
                for(uint c = 1; c < 10; ++c) {
                    int v = 0 ;
                    ins >> v ;
                    set(r, c, v);
                    if (!ins) {
                        throw std::runtime_error("Sudoku initial state given in invalid format.");
                    }
                }
            }
        }

        void print(std::ostream& out) const {
            for(uint r = 0; r < 9; ++r) {
                for( uint c = 0; c < 9; ++c) {
                    out << std::setw(2) << sudoku[r][c] ;
                }
                out << std::endl ;
            }
        }

        uint countUndo() const {
            return numundo ;
        }

        void set(int r, int c, int v) {
                if (v == 0) {
                    return ;
                }
                if (v > 0) {
                    iset(r-1,c-1,v);
                    tset(r-1,c-1,v);
                } else {
                    validation=true;
                    tset(r-1,c-1,-v);
                }
        }

        bool forValidation() const {
            return validation ;
        }

        void verify(std::ostream& out) const {
            uint numbad=0 ;
            for(uint r = 0; r < 9; ++r) {
                for( uint c = 0; c < 9; ++c) {
                    if (sudoku_test[r][c] > 0) {
                        if (sudoku_test[r][c] == sudoku[r][c]) {
                            out << std::setw(2) << sudoku[r][c] ;
                        } else {
                           numbad++;
                           out << std::setw(2) << 'X' ;
                        }
                    } else {
                           out << std::setw(2) << '.' ;
                    }
                }
                out << std::endl ;
            }
        }

        bool solve() {
            uint r_opt = 0 ;
            uint c_opt = 0 ;
            uint m_opt = 0 ;
            uint v_opt = 0 ;
            uint x_opt = 0 ;
            bool done  = true ;
            for(uint r = 0; r < 9; ++r) {
                for(uint c = 0; c < 9; ++c) {
                    if (sudoku[r][c] != 0) {
                        continue ;
                    }
                    done = false ;
                    uint v = (row[r]|col[c]|blk[3*int(r/3)+int(c/3)]) ;
                    uint m = countbits(v);
                    x_opt = std::max(x_opt, 9 - m);
                    if (m > m_opt && m != 9) {
                        r_opt = r ;
                        c_opt = c ;
                        v_opt = v ;
                        m_opt = m ;
                    }
                }
            }
            if (done == true) {
                return true ;
            }
            if (x_opt == 0) {
                return false ;
            }
            v_opt >>= 1 ;
            for(uint v = 1; v < 10; ++v) {
                if ((v_opt & 1) == 0) {
                    iset(r_opt, c_opt, v);
                    if (solve()) {
                        return true ;
                    }
                    undo();
                }
                v_opt >>= 1;
            }
            return false ;
        }
};

std::ostream& operator<<(std::ostream& out, const Sudoku& sudoku) {
    if (sudoku.forValidation()) {
        sudoku.verify(out);
    } else {
        sudoku.print(out) ;
    }
    return out ;
};

int main(int c, const char* v[]) {
        Sudoku sudoku ;
        sudoku.init(std::cin);
        if (sudoku.solve()) {
			std::cout << sudoku << std::endl;
			std::cout << "Undo: " << sudoku.countUndo() << std::endl ;
        } else {
			std::cout << "no solution" << std::endl;
        }
        return 0;
}


