unittest = require "unittest"

function test_class_exists()
    assert(type(class) == "function", "class function not defined")
end

function test_basic_class()
    local Animal = class()
    assert(type(Animal) == "table", "class() should return a table")
    assert(type(Animal.new) == "function", "class should have new()")
end

function test_class_with_init()
    local Animal = class()
    function Animal:init(name, sound)
        self.name = name
        self.sound = sound
    end
    function Animal:speak()
        return self.name .. " says " .. self.sound
    end

    local dog = Animal.new("Dog", "Woof")
    assert(dog.name == "Dog", "name not set")
    assert(dog.sound == "Woof", "sound not set")
    assert(dog:speak() == "Dog says Woof", "method not working")
end

function test_class_inheritance()
    local Animal = class()
    function Animal:init(name)
        self.name = name
    end
    function Animal:speak()
        return self.name .. " speaks"
    end

    local Dog = class(Animal)
    function Dog:init(name, trick)
        Animal.init(self, name)
        self.trick = trick
    end
    function Dog:do_trick()
        return self.name .. " does " .. self.trick
    end

    local d = Dog.new("Rex", "roll over")
    assert(d.name == "Rex", "inherited init not working")
    assert(d:speak() == "Rex speaks", "inherited method not working")
    assert(d:do_trick() == "Rex does roll over", "own method not working")
end

function test_super_field()
    local Base = class()
    local Derived = class(Base)
    assert(Derived.super == Base, "super should reference base class")
end

function test_class_with_proxy_base()
    local SpecialPoint = class(mge.point)
    assert(SpecialPoint.super == mge.point, "super should be mge.point")
    assert(type(SpecialPoint.new) == "function", "should have new()")

    function SpecialPoint:init(x, y, label)
        self._native = mge.point(x, y)
        self.label = label
    end

    local sp = SpecialPoint.new(10, 20, "origin")
    assert(sp.label == "origin", "label not set")
    assert(sp.x == 10, "x coordinate not correct")
    assert(sp.y == 20, "y coordinate not correct")
end

unittest.main()
