#pragma once

#include <random>
#include <iostream>

namespace studd
{
    template<class RealType = double>
    class wrapped_normal_distribution
    {
    public:
        class param_type
        {
        public:
            param_type(RealType min = 0.0, RealType max = 0.0,
                       RealType mean = 0.0, RealType stddev = 1.0)
                : min_(min), max_(max), mean_(mean), stddev_(stddev)
            { };

            RealType min() const { return min_; };
            RealType max() const { return max_; };
            RealType mean() const { return mean_; };
            RealType stddev() const { return stddev_; };

            friend bool operator==(const param_type& lhs, const param_type& rhs)
            {
                return lhs.min() == rhs.min()
                    && lhs.max() == rhs.max()
                    && lhs.mean() == rhs.mean()
                    && lhs.stddev() == rhs.stddev();
            }
            friend bool operator!=(const param_type& lhs, const param_type& rhs)
            {
                return !(lhs == rhs);
            }

        private:
            RealType min_;
            RealType max_;
            RealType mean_;
            RealType stddev_;
        };

        using result_type = RealType;

        wrapped_normal_distribution(RealType min = 0.0, RealType max = 0.0,
                                    RealType mean = 0.0, RealType stddev = 1.0)
            : wrapped_normal_distribution(param_type(min, max, mean, stddev))
        { };
        wrapped_normal_distribution(const param_type& params)
            : params_(params), normal(params.mean(), params.stddev())
        { };

        template<class Generator>
        result_type operator()(Generator& g, const param_type& params)
        {
            return wrap(normal(g), params);
        }
        template<class Generator>
        result_type operator()(Generator& g)
        {
            return operator()(g, param());
        }

        void reset() { };

        param_type param() const { return params_; };
        void param(const param_type& params) { params_ = params; };
        result_type min() const { return param().min(); };
        result_type max() const { return param().max(); };
        result_type mean() const { return param().mean(); };
        result_type stddev() const { return param().stddev(); };

        friend bool operator==(const wrapped_normal_distribution<RealType>& lhs,
                               const wrapped_normal_distribution<RealType>& rhs)
        {
            return lhs.param() == rhs.param();
        }
        friend bool operator!=(const wrapped_normal_distribution<RealType>& lhs,
                               const wrapped_normal_distribution<RealType>& rhs)
        {
            return !(lhs == rhs);
        }

    private:
        static result_type wrap(result_type value, const param_type& params)
        {
            auto diff = params.max() - params.min();
            while (value < params.min())
                value += diff;
            while (value > params.max())
                value -= diff;
            return value;
        }

        param_type params_;
        std::normal_distribution<result_type> normal;
    };


    template<class CharT, class Traits, class RealType>
    std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& ost,
                                                  const wrapped_normal_distribution<RealType>& d)
    {
        ost << d.min() << " "
            << d.max() << " "
            << d.mean() << " "
            << d.stddev();

        return ost;
    }

    template<class CharT, class Traits, class RealType>
    std::basic_istream<CharT, Traits>& operator>>(std::basic_istream<CharT, Traits>& ist,
                                                  wrapped_normal_distribution<RealType>& d)
    {
        RealType min;
        ist >> min;
        RealType max;
        ist >> max;
        RealType mean;
        ist >> mean;
        RealType stddev;
        ist >> stddev;
        d.param(typename wrapped_normal_distribution<RealType>::param_type(min, max, mean, stddev));

        return ist;
    }
}