#include "poly.h"
using namespace std;

// Slaton Ransom Spangler
// Homework 4, Part 1
// TA: James Balasalle



/*
 * In your poly.cxx, write a clear description of how the member variables of a
polynomial are used.
* 
* The head pointer always points to the 0th degree term of any given polynomial. This shouldn't really move.
* The tail pointer always points to the biggest term. This should be adjusted when we add a bigger term.
* The recent pointer points to the last used node. It's used for keeping track of things.
* The current_degree should tell us what the current exponent is. It should be set whenever we adjust
* the tail pointer.
* 
* 

 */


namespace main_savitch_5
{
	polynomial::polynomial(double c, unsigned int exponent)
	{
		// store machine epsilon
		EPSILON = std::numeric_limits<double>::epsilon();
		// write the rest
		
		// four cases
		if(fabs(c) < EPSILON && exponent == 0){
			head_ptr = new polynode();
			current_degree = 0;
			tail_ptr = head_ptr;
			set_recent(exponent);
		}
		else if(fabs(c) > EPSILON && exponent == 0){
			head_ptr = new polynode(c, exponent, nullptr, nullptr);
			current_degree = 0;
			tail_ptr = head_ptr;
			set_recent(exponent);
		}
		else if(fabs(c) < EPSILON && exponent > 0){
			head_ptr = new polynode();
			current_degree = 0;
			tail_ptr = head_ptr;
			set_recent(exponent);
		}
		else if(fabs(c) > EPSILON && exponent > 0){
			head_ptr = new polynode();
			tail_ptr = new polynode(c, exponent, nullptr, head_ptr);
			head_ptr->set_fore(tail_ptr);
			recent_ptr = tail_ptr;
			current_degree = exponent;
		}
		else{
			cout<<"???"<<endl; // We should never get here.
		}
		
    }

    polynomial& polynomial::operator=(const polynomial& source)
    {
		EPSILON = std::numeric_limits<double>::epsilon();
		if (this == &source)
			return *this;
		if (head_ptr != nullptr)
		{	
			clear();
			delete head_ptr;
		}
		tail_ptr = nullptr;
		head_ptr = nullptr;
		recent_ptr = nullptr;
	
		if (source.head_ptr != nullptr)
		{
			head_ptr = new polynode(source.head_ptr->coef(), source.head_ptr->exponent());
			tail_ptr = head_ptr;
			recent_ptr = head_ptr;
			current_degree = 0;
	
			for (unsigned int expo = source.next_term(0); expo != 0; expo = source.next_term(expo))
				assign_coef(source.coefficient(expo), expo);
		}
		return *this;
	}
	
    polynomial::polynomial(const polynomial& source) // pretty much the same as = operator
    {
		// store machine epsilon
		EPSILON = std::numeric_limits<double>::epsilon();
		// write the rest
			
		recent_ptr = source.head_ptr;
		
		if(recent_ptr == nullptr){
			head_ptr = nullptr; // making sure it's a deep copy
			tail_ptr = nullptr;
			current_degree = 0;
		}
		
		else{
			
			double c = recent_ptr->coef();
			unsigned int exp = recent_ptr->exponent();
			head_ptr = new polynode(c, exp, recent_ptr->fore(), recent_ptr->back());
			
			recent_ptr = recent_ptr->fore();
			polynode* curr = head_ptr;
			
			while(recent_ptr != nullptr){
				c = recent_ptr->coef();
				exp = recent_ptr->exponent();
				curr->set_fore(new polynode(c, exp, recent_ptr->fore(), recent_ptr->back()));
				curr = curr->fore();
				recent_ptr = recent_ptr->fore();
			}
			tail_ptr = curr;
			
			current_degree = curr->exponent();
		}
		
    }

    polynomial::~polynomial()
    {
		clear();
		delete head_ptr;
		head_ptr = nullptr;
		tail_ptr = nullptr;
		recent_ptr = nullptr;
		current_degree = 0;
    }

    void polynomial::clear()
    {	
		polynode* curr_node = head_ptr;
		while (head_ptr->fore() != nullptr)
		{
			curr_node = head_ptr->fore();
			delete head_ptr;
			head_ptr = curr_node;
		}
		head_ptr->set_coef(0);
		head_ptr->set_exponent(0);
		current_degree = 0;
		tail_ptr = head_ptr;
		recent_ptr = head_ptr;
		
    }
    
    double polynomial::coefficient(unsigned int exponent) const
    {
		set_recent(exponent);
		if(recent_ptr->exponent() == exponent){
			return recent_ptr->coef();
		}

	return 0.0;
    }

    void polynomial::add_to_coef(double amount, unsigned int exponent)
    {
		double c = amount + coefficient(exponent);
		assign_coef(c, exponent);
	}

    void polynomial::assign_coef(double coefficient, unsigned int exponent)
    {
		set_recent(exponent);
		if(exponent == 0){
			recent_ptr->set_coef(coefficient);
		}
		else if(fabs(coefficient) < EPSILON){
			if(recent_ptr->exponent() == exponent){
				//remove node
				(recent_ptr->back())->set_fore(recent_ptr->fore());
				if(recent_ptr != tail_ptr){
					(recent_ptr->fore())->set_back(recent_ptr->back());
				}
				else{
					tail_ptr = recent_ptr->back();
					current_degree = tail_ptr->exponent();
				}
				
				polynode *to_delete = recent_ptr;
				recent_ptr = recent_ptr->back();
				delete to_delete;
			}
		}
			
		else{
			if(recent_ptr->exponent() == exponent){
				recent_ptr->set_coef(coefficient);
			}
			else{
				recent_ptr = new polynode(coefficient, exponent, recent_ptr->fore(), recent_ptr);				
				recent_ptr->back()->set_fore(recent_ptr);
				if(recent_ptr->fore() != nullptr){
					recent_ptr->fore()->set_back(recent_ptr);
				}
				else{
					tail_ptr = recent_ptr;
					current_degree = exponent;
				}
				
			}
		}
			
		
    }

    unsigned int polynomial::next_term(unsigned int exponent) const
    {
			set_recent(exponent);
			if(recent_ptr->fore() != nullptr){
				return recent_ptr->fore()->exponent();
			}
			return 0;
    }

	unsigned int polynomial::previous_term(unsigned int exponent) const
    {
		
		if(exponent == 0){
			return UINT_MAX;
		}
		
		set_recent(exponent - 1);
		
		if(recent_ptr->coef() < EPSILON){
			return UINT_MAX;
		}
		
		return recent_ptr->exponent();
    }
    
    void polynomial::set_recent(unsigned int exponent) const
    {
		if(exponent == 0){
			recent_ptr = head_ptr;
		}
		else if(exponent >= current_degree){
			recent_ptr = tail_ptr;
		}
		else if(exponent < recent_ptr->exponent()){
			while(recent_ptr->back() != nullptr && recent_ptr->exponent() > exponent){
				recent_ptr = recent_ptr->back();
			}
			// move recent_ptr back some
		}
		else{
			while(recent_ptr->fore() != nullptr && recent_ptr->exponent() < exponent){
				recent_ptr = recent_ptr->fore();
			}
			// move recent_ptr forward some
		}
		
	}
    
    double polynomial::eval(double x) const
    {
		double total = 0;
		unsigned int ex = next_term(0);
		double i;
		while(ex != 0){
			i = ex;
			total = ((pow(x, i) * coefficient(ex)) + total);
			ex = next_term(ex);
		}
		return total;
    }

    polynomial polynomial::derivative() const
    {
		polynomial p_prime;
		unsigned int exp_1 = next_term(0);
		double i;
		while(exp_1 != 0){
			i = exp_1;
			p_prime.assign_coef((coefficient(exp_1) * i), (exp_1 - 1));
			exp_1 = next_term(exp_1);
		}
		return p_prime;
    }
    
    polynomial operator+(const polynomial& p1, const polynomial& p2)
    {
		polynomial p;
		
		unsigned int exp_1 = p1.next_term(0);
		unsigned int exp_2 = p2.next_term(0);
		p.assign_coef(p1.coefficient(0) + p2.coefficient(0), 0);
		while(exp_1 != 0 && exp_2 != 0){
			if(exp_1 == exp_2){
				p.assign_coef(p1.coefficient(exp_1) + p2.coefficient(exp_2), exp_1);
				exp_1 = p1.next_term(exp_1);
				exp_2 = p2.next_term(exp_2);
			}
			else if(exp_1 < exp_2){
				p.assign_coef(p1.coefficient(exp_1), exp_1);
				exp_1 = p1.next_term(exp_1);
			}
			else if(exp_2 < exp_1){
				p.assign_coef(p2.coefficient(exp_2), exp_2);
				exp_2 = p2.next_term(exp_2);
			}
		}
			while(exp_1 != 0){
				p.assign_coef(p1.coefficient(exp_1), exp_1);
				exp_1 = p1.next_term(exp_1);
			}
			
			while(exp_2 != 0){
				p.assign_coef(p2.coefficient(exp_2), exp_2);
				exp_2 = p2.next_term(exp_2);
			}
				
		
		return p;
    }
    
    polynomial operator-(const polynomial& p1, const polynomial& p2)
    {
		polynomial p;
		unsigned int exp_1 = p1.next_term(0);
		unsigned int exp_2 = p2.next_term(0);
		p.assign_coef(p1.coefficient(0) - p2.coefficient(0), 0);
		while(exp_1 != 0 && exp_2 != 0){
			if(exp_1 == exp_2){
				p.assign_coef(p1.coefficient(exp_1) - p2.coefficient(exp_2), exp_1);
				exp_1 = p1.next_term(exp_1);
				exp_2 = p2.next_term(exp_2);
			}
			else if(exp_1 < exp_2){
				p.assign_coef(p1.coefficient(exp_1), exp_1);
				exp_1 = p1.next_term(exp_1);
			}
			else if(exp_2 < exp_1){
				p.assign_coef(-p2.coefficient(exp_2), exp_2);
				exp_2 = p2.next_term(exp_2);
			}
		}
			while(exp_1 != 0){
				p.assign_coef(p1.coefficient(exp_1), exp_1);
				exp_1 = p1.next_term(exp_1);
			}
			
			while(exp_2 != 0){
				p.assign_coef(p2.coefficient(exp_2), exp_2);
				exp_2 = p2.next_term(exp_2);
			}
				
		
		return p;
    }
    
    polynomial operator*(const polynomial& p1, const polynomial& p2)
    {		
		polynomial p;
		unsigned int exp_1 = p1.next_term(0);
		unsigned int exp_2 = p2.next_term(0);
		p.assign_coef(p1.coefficient(0) * p2.coefficient(0), 0);
		while(exp_2 != 0){
			p.assign_coef(p1.coefficient(0) * p2.coefficient(exp_2), exp_2);
			exp_2 = p2.next_term(exp_2);
		}
		exp_2 = p2.next_term(0);
		while(exp_1 != 0){
			while(exp_2 != 0){
				p.add_to_coef(p1.coefficient(exp_1) * p2.coefficient(exp_2), (exp_1 + exp_2));
				exp_2 = p2.next_term(exp_2);
			}
			exp_1 = p1.next_term(exp_1);
			exp_2 = p2.next_term(exp_2);
		}
		
		return p;
    }

    ostream& operator << (ostream& out, const polynomial& p)
    {
		unsigned int expo = 0;
		do{
			out << p.coefficient(expo) << "*x^" << expo;
			expo = p.next_term(expo);
			if (expo != 0) out << " + ";
		} while(expo != 0);

		return out;
    }
    
    void polynomial::find_root(
	double& answer,
	bool& success,
	unsigned int& iterations,
	double guess,
	unsigned int maximum_iterations,
	double epsilon) const
    {
		success = false;
		double q = 10000;
		int i = guess;
		double curr = eval(i);
		while(iterations != maximum_iterations && (fabs(curr) > epsilon || fabs(curr) < -epsilon)){
			if(curr > 0){
				if(q < 0){
					q = -q;}
				while(curr < q){
					q = q/10;}
				i = i - (q / pow(10, current_degree));
				curr = eval(i);}
			else{
				if(q > 0){
					q = -q;}
				while(curr > q){
					q = q/10;}
				i = i + (q/ pow(10, current_degree));
				curr = eval(i);}}
		answer = i;
		if(fabs(curr) < epsilon && fabs(curr) > -epsilon){
			success = true;}
		else{
			success = false;}
	}
}
