#include <limits>

#include "timestamp.h"

namespace av {

Timestamp::Timestamp() noexcept
{
}

Timestamp::Timestamp(int64_t timestamp, const Rational &timebase) noexcept
    : m_timestamp(timestamp),
      m_timebase(timebase)
{
}

int64_t Timestamp::timestamp() const noexcept
{
    return m_timestamp;
}

int64_t Timestamp::timestamp(const Rational &timebase) const noexcept
{
    return m_timebase.rescale(m_timestamp, timebase);
}

const Rational &Timestamp::timebase() const noexcept
{
    return m_timebase;
}

bool Timestamp::isValid() const noexcept
{
    return m_timestamp != AV_NOPTS_VALUE;
}

bool Timestamp::isNoPts() const noexcept
{
    return m_timestamp == AV_NOPTS_VALUE;
}

Timestamp::operator double() const noexcept
{
    return seconds();
}

double Timestamp::seconds() const noexcept
{
    if (isNoPts()) {
        return std::numeric_limits<double>::quiet_NaN();
    }
    return m_timebase.getDouble() * m_timestamp;
}

static int64_t av_add_stable(AVRational ts_tb, int64_t ts, AVRational inc_tb, int64_t inc)
{
    int64_t m, d;

    if (inc != 1)
        inc_tb = av_mul_q(inc_tb, (AVRational) {static_cast<int>(inc), 1});

    m = inc_tb.num * (int64_t)ts_tb.den;
    d = inc_tb.den * (int64_t)ts_tb.num;

    if (m % d == 0)
        return ts + m / d;
    if (m < d)
        return ts;

    {
        int64_t old = av_rescale_q(ts, ts_tb, inc_tb);
        int64_t old_ts = av_rescale_q(old, inc_tb, ts_tb);
        return av_rescale_q(old + 1, inc_tb, ts_tb) + (ts - old_ts);
    }
}

Timestamp &Timestamp::operator+=(const Timestamp &other)
{
    m_timestamp = av_add_stable(m_timebase, m_timestamp,
                                other.timebase(), other.timestamp());
    return *this;
}

Timestamp &Timestamp::operator-=(const Timestamp &other)
{
    auto tmp = *this - other;
    m_timestamp = tmp.timestamp(m_timebase);
    return *this;
}

Timestamp &Timestamp::operator*=(const Timestamp &other)
{
    auto tmp = *this * other;
    m_timestamp = tmp.timestamp(m_timebase);
    return *this;
}

Timestamp &Timestamp::operator/=(const Timestamp &other)
{
    auto tmp = *this / other;
    m_timestamp = tmp.timestamp(m_timebase);
    return *this;
}

Timestamp::operator bool() const noexcept
{
    return m_timestamp != AV_NOPTS_VALUE;
}

Timestamp operator+(const Timestamp &left, const Timestamp &right) noexcept
{
    // Use more good precision
    if (left.timebase() < right.timebase()) {
        auto ts = av_add_stable(left.timebase(), left.timestamp(),
                                right.timebase(), right.timestamp());
        return {ts, left.timebase()};
    } else {
        auto ts = av_add_stable(right.timebase(), right.timestamp(),
                                left.timebase(), left.timestamp());
        return {ts, right.timebase()};
    }
}

Timestamp operator-(const Timestamp &left, const Timestamp &right) noexcept
{
    // Use more good precision
    auto tb = std::min(left.timebase(), right.timebase());
    auto tsleft  = left.timebase().rescale(left.timestamp(), tb);
    auto tsright = right.timebase().rescale(right.timestamp(), tb);

    auto ts = tsleft - tsright;

    return {ts, tb};
}

Timestamp operator*(const Timestamp &left, const Timestamp &right) noexcept
{
    auto ts = left.timestamp() * right.timestamp();
    auto tb = left.timebase() * right.timebase();
    return {ts, tb};
}

Timestamp operator/(const Timestamp &left, const Timestamp &right) noexcept
{
    int num, den;

    // Use more good precision
    auto tb = std::min(left.timebase(), right.timebase());
    auto tsleft  = left.timebase().rescale(left.timestamp(), tb);
    auto tsright = right.timebase().rescale(right.timestamp(), tb);

    int64_t ts = 1;
    if (tsleft > tsright) {
        ts = tsleft / tsright;
        tsleft %= tsright;
        if (tsleft == 0) {
            tsleft = 1;
            tsright = 1;
        }
    }

    av_reduce(&num, &den, tsleft, tsright, INT64_MAX);

    return {ts, {num, den}};
}

} // ::av
